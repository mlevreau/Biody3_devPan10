/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

struct timer_typ {
	uint8_t   topms;
	uint8_t   top10ms;
	uint8_t   top100ms;
	uint8_t   topsec;
	uint32_t  pxms;
	uint32_t  ppms;
	uint32_t  px10ms;
	uint32_t  pp10ms;
	uint32_t  px100ms;
	uint32_t  pp100ms;
	uint32_t  pps;
};


#define USB_uart 		huart3
#define BLE_uart 		huart2

extern volatile struct timer_typ timer;


/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define SOFT_VERSION "V3.0.002"

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define min(a,b) ((a>b)?(b):(a))
#define max(a,b) ((a<b)?(b):(a))

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BlueBtn_Pin GPIO_PIN_13
#define BlueBtn_GPIO_Port GPIOC
#define BlueBtn_EXTI_IRQn EXTI15_10_IRQn
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define STLINK_RX_Pin GPIO_PIN_8
#define STLINK_RX_GPIO_Port GPIOD
#define STLINK_TX_Pin GPIO_PIN_9
#define STLINK_TX_GPIO_Port GPIOD
#define BLE_CTS_Pin GPIO_PIN_3
#define BLE_CTS_GPIO_Port GPIOD
#define BLE_RTS_Pin GPIO_PIN_4
#define BLE_RTS_GPIO_Port GPIOD
#define BLE_TX_Pin GPIO_PIN_5
#define BLE_TX_GPIO_Port GPIOD
#define BLE_RX_Pin GPIO_PIN_6
#define BLE_RX_GPIO_Port GPIOD
#define BLE_RST_Pin GPIO_PIN_7
#define BLE_RST_GPIO_Port GPIOD
#define LD2_Pin GPIO_PIN_1
#define LD2_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
