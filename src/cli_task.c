#include <string.h>

#include "FreeRTOS.h"
#include "FreeRTOS_CLI.h"
#include "asf.h"
#include "cli_task.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#define CLI_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define CLI_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE + 10)

static SemaphoreHandle_t cdc_new_data = NULL;
static volatile bool cli_task_cdc_enabled = false;
static unsigned long ulClocksPer10thOfAMilliSecond = 0UL;

/* FreeRTOS_CLI command prototype */
static long hello_world_cmd(char *buffer, size_t buf_len,
                            const char *cmd_string);

static long task_stats_cmd(char *buffer, size_t buf_len,
                           const char *cmd_string);

static long run_time_stats_cmd(char *buffer, size_t buf_len,
                               const char *cmd_string);

/* FreeRTOS_CLI command definition */
static const CLI_Command_Definition_t hello_world_def = {
    "hello", /* The command string to type */
    "hello:\r\n Echos back \"world\"\r\n",
    hello_world_cmd, /* The function to run. */
    0                /* No paramters are expected. */
};

static const CLI_Command_Definition_t task_stats_def = {
    "task-stats", /* The command string to type. */
    "\r\ntask-stats:\r\n Displays a table showing the state of each FreeRTOS "
    "task\r\n",
    task_stats_cmd, /* The function to run. */
    0               /* No parameters are expected. */
};

static const CLI_Command_Definition_t run_time_stats_def = {
    "run-time-stats", /* The command string to type. */
    "\r\nrun-time-stats:\r\n Displays a table showing how much processing time "
    "each FreeRTOS task has used\r\n",
    run_time_stats_cmd, /* The function to run. */
    0                   /* No parameters are expected. */
};

static void cli_task(void *params) {
    char *out_buffer, in_buffer[64] = {0};
    const char *const err_input = "\r\ninvalid input\r\n";
    size_t index = 0;
    long cmd_continue;
    char ch;

    out_buffer = FreeRTOS_CLIGetOutputBuffer();

    while (true) {
        /* wait for input data */
        xSemaphoreTake(cdc_new_data, portMAX_DELAY);
        while (udi_cdc_is_rx_ready() == true) {
            /* NOTE: We ignore control character sequences, like arrow keys and
             * delete keys. Backspace is supported. */
            ch = udi_cdc_getc();
            switch (ch) {
            case '\r':
            case '\n':
                /* if enter is pressed, start new line and process command */
                udi_cdc_write_buf("\r\n", strlen("\r\n"));
                do {
                    /* send null terminated input string to FreeRTOS_CLI */
                    cmd_continue = FreeRTOS_CLIProcessCommand(
                        in_buffer, out_buffer,
                        configCOMMAND_INT_MAX_OUTPUT_SIZE);

                    /* transmit result
                    string in buffer should be null terminated */
                    udi_cdc_write_buf(out_buffer, strlen(out_buffer));
                } while (cmd_continue);

                /* clear input buffer */
                memset(in_buffer, 0, sizeof(in_buffer));
                index = 0;
                break;
            case '\b':
            case 0x7F:
                /* delete last character and echo backspace */
                index -= (index > 0) ? 1 : 0;
                in_buffer[index] = '\0';
                udi_cdc_putc('\b');
                break;
            case '-':
            case ' ':
            case 'A' ... 'Z':
            case 'a' ... 'z':
                /* add input to buffer
                increment index if smaller then buffer size */
                in_buffer[index] = ch;
                index += (index < sizeof(in_buffer)) ? 1 : 0;
                /* echo back input */
                udi_cdc_putc(ch);
                break;
            default:
                udi_cdc_write_buf(err_input, strlen(err_input));
            }
        }
    }
}

void cli_task_init() {

    vSemaphoreCreateBinary(cdc_new_data);
    configASSERT(cdc_new_data);

    FreeRTOS_CLIRegisterCommand(&hello_world_def);
    FreeRTOS_CLIRegisterCommand(&task_stats_def);
    FreeRTOS_CLIRegisterCommand(&run_time_stats_def);

    udc_start();

    xTaskCreate(cli_task, (const char *)"CLI", CLI_TASK_STACK_SIZE, NULL,
                CLI_TASK_PRIORITY, NULL);
}

bool cli_task_cdc_enable(uint8_t port) {
    cli_task_cdc_enabled = true;
    return true;
}

void cli_task_cdc_disable(uint8_t port) { cli_task_cdc_enabled = false; }

void cli_task_cdc_rx_notify(uint8_t port) {
    long higher_priority_task_woken = false;
    configASSERT(cdc_new_data);
    xSemaphoreGiveFromISR(cdc_new_data, &higher_priority_task_woken);
    portEND_SWITCHING_ISR(higher_priority_task_woken);
}

void vMainConfigureTimerForRunTimeStats(void) {
    /* define how many clock ticks occur in 1/10th of a millisecond */
    ulClocksPer10thOfAMilliSecond = configCPU_CLOCK_HZ / 10000UL;
}

unsigned long ulMainGetRunTimeCounterValue(void) {
    unsigned long ulSysTickCounts, ulTickCount, ulReturn;
    const unsigned long ulSysTickReloadValue =
        (configCPU_CLOCK_HZ / configTICK_RATE_HZ) - 1UL;
    volatile unsigned long *const pulCurrentSysTickCount =
        ((volatile unsigned long *)0xe000e018);
    volatile unsigned long *const pulInterruptCTRLState =
        ((volatile unsigned long *)0xe000ed04);
    const unsigned long ulSysTickPendingBit = 0x04000000UL;

    /* Used by the optional run-time stats gathering functionality. */

    /* NOTE: There are potentially race conditions here.  However, it is used
    anyway to keep the examples simple, and to avoid reliance on a separate
    timer peripheral. */

    /* The SysTick is a down counter.  How many clocks have passed since it was
    last reloaded? */
    ulSysTickCounts = ulSysTickReloadValue - *pulCurrentSysTickCount;

    /* How many times has it overflowed? */
    ulTickCount = xTaskGetTickCountFromISR();

    /* This is called from the context switch, so will be called from a
    critical section.  xTaskGetTickCountFromISR() contains its own critical
    section, and the ISR safe critical sections are not designed to nest,
    so reset the critical section. */
    portSET_INTERRUPT_MASK_FROM_ISR();

    /* Is there a SysTick interrupt pending? */
    if ((*pulInterruptCTRLState & ulSysTickPendingBit) != 0UL) {
        /* There is a SysTick interrupt pending, so the SysTick has overflowed
        but the tick count not yet incremented. */
        ulTickCount++;

        /* Read the SysTick again, as the overflow might have occurred since
        it was read last. */
        ulSysTickCounts = ulSysTickReloadValue - *pulCurrentSysTickCount;
    }

    /* Convert the tick count into tenths of a millisecond.  THIS ASSUMES
    configTICK_RATE_HZ is 1000! */
    ulReturn = (ulTickCount * 10UL);

    /* Add on the number of tenths of a millisecond that have passed since the
    tick count last got updated. */
    ulReturn += (ulSysTickCounts / ulClocksPer10thOfAMilliSecond);

    return ulReturn;
}

/* FreeRTOS_CLI command implementations */
static long hello_world_cmd(char *buffer, size_t buf_len,
                            const char *cmd_string) {
    strcpy(buffer, "world\r\n");
    return false;
}

static long task_stats_cmd(char *buffer, size_t buf_len,
                           const char *cmd_string) {
    const char *const pcHeader = "Task          State  Priority  Stack	"
                                 "#\r\n****************************************"
                                 "********\r\n";

    /* Remove compile time warnings about unused parameters, and check the
    write buffer is not NULL.  NOTE - for simplicity, this example assumes the
    write buffer length is adequate, so does not check for buffer overflows. */
    (void)cmd_string;
    (void)buf_len;
    configASSERT(buffer);

    /* Generate a table of task stats. */
    strcpy(buffer, pcHeader);
    vTaskList(buffer + strlen(pcHeader));

    /* There is no more data to return after this single string, so return
    pdFALSE. */
    return pdFALSE;
}

static long run_time_stats_cmd(char *buffer, size_t buf_len,
                               const char *cmd_string) {
    const char *const pcHeader = "Task            Abs Time      % "
                                 "Time\r\n*************************************"
                                 "***\r\n";

    /* Remove compile time warnings about unused parameters, and check the
    write buffer is not NULL.  NOTE - for simplicity, this example assumes the
    write buffer length is adequate, so does not check for buffer overflows. */
    (void)cmd_string;
    (void)buf_len;
    configASSERT(buffer);

    /* Generate a table of task stats. */
    strcpy(buffer, pcHeader);
    vTaskGetRunTimeStats(buffer + strlen(pcHeader));

    /* There is no more data to return after this single string, so return
    pdFALSE. */
    return pdFALSE;
}
