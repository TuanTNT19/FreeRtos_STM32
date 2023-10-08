#include "stdio.h"
#include "main.h"
#include "cmsis_os.h"

osThreadId defaultTaskHandle;
TaskHandle_t AboveNormalHandle;
QueueHandle_t QueuexHandle;
void StartDefaultTask(void const * argument);
void AboveNormalTask (void *parameter);
void ADC1_Config()
{
	RCC->APB2ENR |= 1<<9;  // enable ADC1 clock
	RCC->APB2ENR |= (1<<2);  // enable GPIOA clock
	GPIOA->CRL &=(0xFFFFFF0F);
	GPIOA->CRL &=~(1<<1);// set GPIOA_PIN1 as ADC1_Channel 1
	GPIOA->CRL &=(0xFFF0FFFF);
	GPIOA->CRL &=~(1<<4);// set GPIOA_PIN4 as ADC1_Channel 4
	ADC1->CR1 |=(1<<8);//enable scan mode
	ADC1->CR2|=(1<<1);//enable continous mode
	ADC1->CR2|=(1<<8);//enble DMA
	ADC1->CR2 &=~(1<<11); //right alignment
	ADC1->CR2 |=(7<<17);//SWSTART in EXTSEL bit
	ADC1->CR2|=(1<<20);//Conversion on external event enabled
	ADC1->SMPR2 &= ~((7<<3) | (7<<12));  // Sampling time of 1.5 cycles for channel 1 and channel 4
	ADC1->SQR1|=(1<<20);//2 conversions
	ADC1->SQR3|=((1<<0)|(4<<5));//set up ADC channel for channel 1 and 4
	ADC1->CR2 |= (1 << 2); // turn calibrate on 
	while(!(ADC1->CR2 & (1 << 2)));//check calibrate
	ADC1->CR2|=(1<<0);//enable ADC1
	ADC1->CR2 |=(1<<22);//enable conversion
}
void DMA_Init()
{
	RCC->AHBENR |= 1<<0;//enable clock for DMA1
	DMA1_Channel1->CCR &=~ (1<<4);//read from peripheral
	DMA1_Channel1->CCR |=(1<<5);//enable circular mode
	DMA1_Channel1->CCR |=(1<<7);//enable memory increment
	DMA1_Channel1->CCR|=(1<<8);//peripheral size : 16 bit
	DMA1_Channel1->CCR|=(1<<10);//memory size : 16 bit
	
}
void DMA1_Config(uint32_t srcAdd, uint32_t destAdd, uint16_t size)
{
	DMA1_Channel1->CNDTR = size;
	DMA1_Channel1->CPAR = srcAdd;
	DMA1_Channel1->CMAR=destAdd;
	DMA1_Channel1->CCR |=(1<<0);//enable channel DMA
}
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


uint16_t adc_value[2];
uint16_t data_receive[2];
char str[10];
int main(void)
{
  ADC1_Config();
	DMA_Init();
	USART1_Config();
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);
  xTaskCreate(AboveNormalTask, "Task01", 128, NULL, osPriorityAboveNormal+3, &AboveNormalHandle);
  QueuexHandle = xQueueCreate(2,2);
  osKernelStart();


  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}


void AboveNormalTask (void *parameter){
	
	DMA1_Config((uint32_t)&ADC1->DR, (uint32_t)adc_value, 2);
	while(1)
	{
		
	  
		xQueueSend(QueuexHandle,&adc_value[0],NULL);
		xQueueSend(QueuexHandle,&adc_value[1],NULL);
		vTaskDelay(200);
	}
}

void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
     xQueueReceive(QueuexHandle,&data_receive[0],osWaitForever);
		 xQueueReceive(QueuexHandle,&data_receive[1],osWaitForever);
     int len1=sprintf(str,"%4d %4d",data_receive[0],data_receive[1]);
		 USART1_Send_String((char *)str);

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