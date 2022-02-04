#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <stdio.h>
#include "pico/stdlib.h"


static QueueHandle_t xQueue = NULL;


void led_task(){
    const uint LED_PIN = 0; 
    uint ValueToBeSend = 0; 
    gpio_init(LED_PIN); 
    gpio_set_dir(LED_PIN,GPIO_OUT);

    while(1){
        gpio_put(LED_PIN,1);
        ValueToBeSend = 1;
        xQueueSend(xQueue ,&ValueToBeSend, 0U);  
        vTaskDelay(100); 
        gpio_put(LED_PIN,0); 
         ValueToBeSend = 0;
        xQueueSend(xQueue ,&ValueToBeSend, 0U);
        vTaskDelay(100);
    }

}


void printValueOnConsoleTask()
{
 
 uint ValueReceivedFromQueue = 0;
 
    while(1)
    {
        xQueueReceive(xQueue,&ValueReceivedFromQueue,portMAX_DELAY);
    
    if (ValueReceivedFromQueue == 1)
         printf("LED ON \n");
    

    else 
        printf("LED OFF \n");

    }
}


void SchedulerTestTask1()

{

    while(1)
    {
        printf("Task 1 is currently running\n");
        //for(int i = 0; i < 20000000; i++){};
        vTaskDelay(100);
    }
}

void SchedulerTestTask2()

{

    while(1)
    {
        printf("Task 2 is currently running\n");
        //for(int i = 0; i < 20000000; i++){};    //with using this form of delay task 2 will only be runned because it has the highst priority . but if we give task 1 and 2 
                                                  //the same priority and using this form of delay we will see that both tasks run in the same time because of time slicing


         vTaskDelay(100);   //when using vtaskdelay this put this task in the blocked stat that mean that task with lower priority as like task 1 will be exectued when this task consum his delay.
                            // with using vtaskdelay task with the highst priority will executed first than the others 
    }
}



int main()
{
stdio_init_all(); 

xQueue = xQueueCreate(1, sizeof(uint));


//xTaskCreate(led_task, "LED_Task",256,NULL,1,NULL);
//xTaskCreate(printValueOnConsoleTask,"Print Value on console Task",256,NULL,1,NULL);

xTaskCreate(SchedulerTestTask1,"Task 1",256,NULL,1,NULL);
xTaskCreate(SchedulerTestTask2,"Task 2",256,NULL,2,NULL);

vTaskStartScheduler();

while(1){};



}