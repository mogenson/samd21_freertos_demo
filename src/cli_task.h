#pragma once

#include <stdbool.h>
#include <stdint.h>

void cli_task_init(void);
bool cli_task_cdc_enable(uint8_t port);
void cli_task_cdc_disable(uint8_t port);
void cli_task_cdc_rx_notify(uint8_t port);
