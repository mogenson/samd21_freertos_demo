/*
 *
 * Copyright (C) 2017 Scansorial Inc. All rights reserved.
 *
 */

#include "FreeRTOS.h"
#include "asf.h"
#include "cli_task.h"
#include "led_task.h"
#include "task.h"

int main(void) {

    // Initialize the board
    system_init();

    // Initialize the tasks
    led_task_init();
    cli_task_init();

    // ..and let FreeRTOS run tasks!
    vTaskStartScheduler();

    // Intentionally left empty
    while (true) {}
}

void vAssertCalled(const char *file, uint32_t line) {
    volatile uint32_t block_var = 0, line_in;
    const char *file_in;

    /* These assignments are made to prevent the compiler optimizing the
    values away. */
    file_in = file;
    line_in = line;
    (void)file_in;
    (void)line_in;

    taskENTER_CRITICAL();
    {
        while (block_var == 0) {
            /* Set block_var to a non-zero value in the debugger to
            step out of this function. */
        }
    }
    taskEXIT_CRITICAL();
}
