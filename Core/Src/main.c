#include "stdio.h"
#include "main.h"
#include "cmsis_os.h"
TaskHandle_t NormalHandle;
TaskHandle_t AboveNormalHandle;
SemaphoreHandle_t Sema1;
void NormalTask(void *para1);
void AboveNormalTask(void *para2);
uint16_t ADC_Value;
void Tim_Config()
{
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //Enable TIMEeR2 clock
	TIM2->ARR=65535;
	TIM2->PSC = 7999;
	TIM2->CR1|=(1<<0);
	while (!(TIM2->SR & TIM_SR_UIF));
}
void delay_ms(uint16_t ms)
{
	
		TIM2->CNT=0;
	while (TIM2->CNT < ms);
}
void GPIO_Config()
{
	
	RCC->APB2ENR|=(1<<3);//enable clock for gpio b
	GPIOB->CRL &=(0xFFFFFFF0);
	GPIOB->CRL |=(2<<0);
	
}
void ADC1_Config()
{
	
	RCC->APB2ENR|=(1<<9);
	RCC->APB2ENR|=(1<<2);//enable clock for GPIOA
	GPIOA->CRL&=(0xFFFFFFF0);
	GPIOA->CRL &=(0<<0);
	ADC1->CR2|=(1<<0);
	ADC1->CR2|=(1<<1);
	ADC1->SQR1&=(0<<20);
	ADC1->SQR3 &=(0<<0);
	ADC1->CR2|=(1<<2);
	while(!(ADC1->CR2)&(1<<2));
	ADC1->CR2|=(1<<0);
	ADC1->CR2|=(1<<22);
	
	
}
uint16_t Read_ADC1()
{
	while(!(ADC1->SR)&(1<<1));
	uint16_t value = ADC1->DR;
	return ADC1->DR;
	
}
int main()
{
	xTaskCreate(NormalTask,"Task 1",128,NULL,0,&NormalHandle);
	xTaskCreate(AboveNormalTask,"Task 2",128,NULL,1,&AboveNormalHandle);
	Sema1=xSemaphoreCreateBinary();
	GPIO_Config();
	ADC1_Config();
	Tim_Config();
	osKernelStart();
	while(1)
	{
		
	
	}
}
void AboveNormalTask(void *para2){
	while(1)
	{
	GPIOB->ODR &=(0<<0);
	ADC_Value = Read_ADC1();
	if (ADC_Value>4000)
	{
		
		xSemaphoreGive(Sema1);
	}
	vTaskDelay(100);
}
}
void NormalTask(void *para1){
	while(1)
	{
	xSemaphoreTake(Sema1,portMAX_DELAY);
	GPIOB->ODR |=(1<<0);
	}
}