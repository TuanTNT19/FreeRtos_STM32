#include "stdio.h"
#include "main.h"
#include "cmsis_os.h"
#include <stdlib.h>
osThreadId defaultTaskHandle;
TaskHandle_t AboveNormalHandle;
QueueHandle_t QueuexHandle;
void StartDefaultTask(void const * argument);
void AboveNormalTask (void *parameter);
void USART1_Config()
{
	RCC->APB2ENR |= (1<<2);//enable clock for gpioa
	GPIOA->CRH&=(0xFFFFFF0F);
	GPIOA->CRH|=(10<<4);//SET gpioa pin 9 as alternate outout function push pull for TX
	RCC->APB2ENR|=(1<<14);//enable clock for USART1
	USART1->BRR=833;
	USART1->CR1|=(1<<3);//Transmit enable
	USART1->CR1|=(1<<13);// USART enable
}	
void USART1_Send_char(char str)
{
	while (!(USART1->SR & USART_SR_TXE));
	USART1->DR = str;
	
}
void USART1_Send_String(char *str)
{
	
	while(*str)
	{
		
		USART1_Send_char(*str++);
	}
}
struct data{
	uint16_t k;
	int l;
	
};
typedef struct data test;
uint16_t k1;
int l1;
uint16_t adc_value[2];
uint16_t data_receive[2];
uint16_t a=5;
uint16_t b;
uint16_t c=0;
char str[10];
int main(void)
{
  USART1_Config();
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);
  xTaskCreate(AboveNormalTask, "Task01", 128, NULL, 3, &AboveNormalHandle);
  QueuexHandle = xQueueCreate(2,sizeof(test));
  osKernelStart();


  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}


void AboveNormalTask (void *parameter){
	
//	DMA1_Config((uint32_t)&ADC1->DR, (uint32_t)adc_value, 2);
	test *Task_data = osPoolAlloc(QueuexHandle);
	
	Task_data->l=9;
	while(1)
	{
		
	  Task_data->k=a;
		xQueueSend(QueuexHandle,&Task_data ,NULL);
		a++;
		vTaskDelay(200);
	}
}

void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
	test receive_data;
  for(;;)
  {
     xQueueReceive(QueuexHandle,&receive_data,osWaitForever);
		 c++;
//     int len1=sprintf(str,"%4d",receive_data.k);
	    k1=receive_data.k;
		  l1=receive_data.l;
		  osPoolFree(QueuexHandle,&receive_data);
		
  }
  /* USER CODE END 5 */
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}