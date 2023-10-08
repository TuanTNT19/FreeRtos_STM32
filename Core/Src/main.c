
#include "stdio.h"
#include "main.h"
#include "cmsis_os.h"
#include <stdlib.h>
osThreadId defaultTaskHandle;
TaskHandle_t AboveNormalHandle;
SemaphoreHandle_t CouSemaxHandle;
uint16_t a=0;
uint16_t b=0;
void StartDefaultTask(void const * argument);
void AboveNormalTask (void *parameter);
uint8_t count1;
uint8_t count2;
int main(void)
{
  
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);
  xTaskCreate(AboveNormalTask, "Task01", 128, NULL, 3, &AboveNormalHandle);
  CouSemaxHandle=	xSemaphoreCreateCounting(18,4) ;
  osKernelStart();


  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}


void AboveNormalTask (void *parameter){


	while(1)
	{
		xSemaphoreGive(CouSemaxHandle);
		a++;	 
		vTaskDelay(1000);
	}
}

void StartDefaultTask(void const * argument)
{

  for(;;)
  {
		count2=(uint8_t)uxSemaphoreGetCount(CouSemaxHandle);
		b++;
		if (b>=10) {
				vTaskSuspend( AboveNormalHandle)	;		
        xSemaphoreTake(CouSemaxHandle,osWaitForever);		
			}
		vTaskDelay(500);
		
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