#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <queue.h>
#include "pico/stdlib.h"
#include "semphr.h"
SemaphoreHandle_t count;

void LED_task()
{
   const uint LED_PIN = 25;
   gpio_init(LED_PIN);
   gpio_set_dir(LED_PIN,GPIO_OUT);
   while(true){
    if(xSemaphoreTake(count,(TickType_t) 10 ) == pdTRUE){
        gpio_put(LED_PIN,1);
        vTaskDelay(100);
    }
    else{
        gpio_put(LED_PIN,0);
    }
   }
}
void button_task(){
    gpio_init(20);
    gpio_set_dir(20,GPIO_IN);
    gpio_pull_up(20);

    while(true){
        if (gpio_get(20) == 0){
            xSemaphoreGive(count);
            vTaskDelay(20);
        }
        else{
            vTaskDelay(1);
        }
    }
}

int main()
{
    stdio_init_all();

    count= xSemaphoreCreateCounting(5,0);

    xTaskCreate(LED_task,"LED Task", 256, NULL, 1, NULL);
    xTaskCreate(button_task,"Button_Task", 256, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1){};
}