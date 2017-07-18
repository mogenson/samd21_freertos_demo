#pragma once

#include <stdbool.h>
#include <stdint.h>

void cli_task_init(void);
bool cli_task_cdc_enable(uint8_t port);
void cli_task_cdc_disable(uint8_t port);
void cli_task_cdc_rx_notify(uint8_t port);

/** @brief implimentation of POSIX putchar()
 *
 * Can be used with output formatting utilities like printf().
 * Directs bytes to USB CDC serial port.
 * Shares hardware access with cli_task for FreeRTOS_CLI.
 *
 * @param c character to print over USB CDC serial port
 * @return the character written cast to an int or -1 on error
 */
int usb_putchar(int c);
