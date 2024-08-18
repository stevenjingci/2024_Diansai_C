#include "dac.h"


DAC_HandleTypeDef hdac;

/* DAC init function */
void MX_DAC_Init(void)
{
 DAC_ChannelConfTypeDef sConfig = {0};

  /** DAC Initialization 
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT1 config 
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT2 config 
  */
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
}
void HAL_DAC_MspInit(DAC_HandleTypeDef* dacHandle)
{
GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(dacHandle->Instance==DAC)
  {
  /* USER CODE BEGIN DAC_MspInit 0 */
  /* USER CODE END DAC_MspInit 0 */
    /* DAC clock enable */
    __HAL_RCC_DAC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**DAC GPIO Configuration    
    PA4     ------> DAC_OUT1
    PA5     ------> DAC_OUT2 */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* USER CODE BEGIN DAC_MspInit 1 */
  /* USER CODE END DAC_MspInit 1 */
  }
}

//uint16_t magnification1[11]={1340,1340,1330,1320,1315,1305,1300,1295,1290,1275,1270};
//uint16_t magnification2[11]={1410,1400,1390,1385,1380,1370,1360,1350,1340,1325,1310};
void dac_value_set(void)
{
	HAL_DAC_SetValue(&hdac,DAC1_CHANNEL_1,DAC_ALIGN_12B_R,160);
	HAL_DAC_SetValue(&hdac,DAC1_CHANNEL_2,DAC_ALIGN_12B_R,160);
	HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
	HAL_DAC_Start(&hdac,DAC_CHANNEL_2);
}