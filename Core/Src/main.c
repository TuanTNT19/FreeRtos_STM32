


#include "stdio.h"
#include "main.h"
#include "cmsis_os.h"
#include <stdlib.h>
#include "timers.h"
osThreadId defaultTaskHandle;
TaskHandle_t AboveNormalHandle;
//TaskHandle_t NormalTaskHandle;
EventGroupHandle_t Group;
EventBits_t Bit = 0;
uint16_t a=0;
uint16_t b=0;
uint8_t RX_data;
#define EVENT0    (1UL << 0)
#define EVENT1    (1UL << 1)
#define EVENT2    (1UL << 2)
void StartDefaultTask(void const * argument);
void AboveNormalTask (void *parameter);
void NormalTask(void *para);
void GPIO_config()
{
	RCC->APB2ENR |= (1<<2);//enable clock for gpioa
	// config gpioa pin 2 as alternative function TX
	GPIOA->CRL &= (0xFFFFF0FF);
	GPIOA-> CRL |= (10<<8);
		// config gpioa pin 3 as alternative function RX
	GPIOA->CRL &= (0xFFFF0FFF);
	GPIOA->CRL |= (8<<12);//in push pull up/down
	GPIOA->ODR |= (1<<3);// pull up
	RCC->APB2ENR |= (1<<4); // enable clock for gpioc
	GPIOC-> CRH &= (0xF0FFFFFF);
	GPIOC-> CRH |= (2<<24);// set gpioc pin 14 as a led

}
void USART2_config()
{
	RCC->APB1ENR |= (1<<17);//enable clock for USART2
	USART2->BRR |= 833;
	USART2->CR1 |= (1<<13);
	USART2->CR1 |= (1<<2); // RE=1.. Enable the Receiver
  USART2->CR1 |= (1<<3);  // TE=1.. Enable Transmitter
	
}
void USART2_Send_char(char str)
{
	while (!(USART2->SR & USART_SR_TXE));
	USART2->DR=str;
}
void USART2_Send_String(char *ch)
{
	while (*ch)
	{
		 USART2_Send_char(*ch++);
	}
}

uint8_t USART_Getchar(void)
{
	
	uint8_t Temp;
	while (!(USART2->SR & USART_SR_RXNE));
	Temp = USART2->DR;
	return Temp;
}
void UART_Getstring(uint8_t *str, uint8_t len)
{
	for (uint8_t i=0;i<len;i++)
	{
		*str=USART_Getchar();
				if (*str=='n')
			{
				*(str+1)=NULL;
				break;
				}
		str++;
  }
}
int main(void)
{
   GPIO_config();
	USART2_config();
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);
  xTaskCreate(AboveNormalTask, "Task01", 128, NULL, 3, &AboveNormalHandle);
 // xTaskCreate(NormalTask,"Task01",128,NULL,2,&NormalTaskHandle);
	Group =	xEventGroupCreate() ;
  osKernelStart();
	


  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}


void AboveNormalTask (void *parameter){

  USART2_Send_String("Start\n");
	while(1)
	{
    Bit =  xEventGroupWaitBits(Group, EVENT0 | EVENT1 | EVENT2, pdTRUE, pdFALSE , osWaitForever);
			if ((Bit & EVENT1) != 0)
		{
			USART2_Send_String("Event 1");
		}
		else	if ((Bit & EVENT2) != 0)
		{
			USART2_Send_String("Event 2");
		}
				else	if ((Bit & EVENT0) != 0)
		{
			USART2_Send_String("Event 0");
		}
	}
}
//void NormalTask(void *para)
//{
//	
//	while(1)
//	{
//		if ((Bit & EVENT1) != 0)
//		{
//			USART2_Send_String("Event 1");
//		}
//		else	if ((Bit & EVENT2) != 0)
//		{
//			USART2_Send_String("Event 2");
//		}
//				else	if ((Bit & EVENT0) != 0)
//		{
//			USART2_Send_String("Event 0");
//		}
//	}
//	
//}

void StartDefaultTask(void const * argument)
{
   
  for(;;)
  {
		RX_data=USART_Getchar();
    if (RX_data=='1')
		{
			Bit = 	xEventGroupSetBits(Group,EVENT1);
			
		}
		else if (RX_data=='2')
		{
			
			Bit = xEventGroupSetBits(Group,EVENT2);
		}
		else {
			Bit = xEventGroupSetBits(Group,EVENT0);
		}
    
		
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