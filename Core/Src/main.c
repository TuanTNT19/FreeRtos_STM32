#include "stdio.h"
#include "main.h"
#include "cmsis_os.h"

osThreadId defaultTaskHandle;
TaskHandle_t AboveNormalHandle;
void StartDefaultTask(void const * argument);
void AboveNormalTask (void *parameter);
int i=0;
	int j=0;
int main(void)
{
 
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);
  xTaskCreate(AboveNormalTask, "Task01", 128, NULL, osPriorityAboveNormal+3, &AboveNormalHandle);

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
		
		i++;
		if (i==5)
		{
			vTaskSuspend(defaultTaskHandle);
		}
		if (i==10)
		{
			vTaskResume(defaultTaskHandle);
			
		}
		vTaskDelay(500);
		
	}
}

void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */

  for(;;)
  {
		j++;
    osDelay(500);
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