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

/* FreeRTOS_CLI command prototype */
static long hello_world_cmd(char *buffer, size_t buf_len,
                            const char *cmd_string);

/* FreeRTOS_CLI command definition */
static const CLI_Command_Definition_t hello_world_def = {
    (const char *const) "hello",
    (const char *const) "hello:\r\n Echos back \"world\"\r\n", hello_world_cmd,
    0};

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

static long hello_world_cmd(char *buffer, size_t buf_len,
                            const char *cmd_string) {
    strcpy(buffer, "world\r\n");
    return false;
}
