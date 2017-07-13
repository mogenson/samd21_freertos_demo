#include "led_task.h"
#include "FreeRTOS.h"
#include "asf.h"
#include "task.h"

#define LED_TASK_PRIORITY (tskIDLE_PRIORITY + 2)
#define LED_TASK_DELAY (500 / portTICK_PERIOD_MS)

static void led_task(void *params) {
    /* this task simulates the main application work
     * it is the highest priority, so nothing else should
     * run until this task goes to sleep */
    while (1) {
        LED_Toggle(LED_0_PIN);
        vTaskDelay(LED_TASK_DELAY);
    }
}

void led_task_init() {
    xTaskCreate(led_task, (const char *)"LED", configMINIMAL_STACK_SIZE, NULL,
                LED_TASK_PRIORITY, NULL);
}
