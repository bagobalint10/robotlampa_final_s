/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define PWM_ON_DUTY 420 	//5us
#define PWM_OFF_DUTY 0
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void usart_transmit(uint8_t *data);

void tim_2_set_duty(uint8_t duty);
void tim_2_set_period(uint32_t period);
uint32_t tim_2_get_value(void);

void tim_5_set_duty(uint8_t duty);
void tim_5_set_period(uint32_t period);
uint32_t tim_5_get_value(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define MOTOR_1_PULSE_Pin GPIO_PIN_0
#define MOTOR_1_PULSE_GPIO_Port GPIOA
#define MOTOR_2_PULSE_Pin GPIO_PIN_1
#define MOTOR_2_PULSE_GPIO_Port GPIOA
#define MOTOR_2_DIRECTION_Pin GPIO_PIN_2
#define MOTOR_2_DIRECTION_GPIO_Port GPIOA
#define MOTOR_1_DIRECTION_Pin GPIO_PIN_3
#define MOTOR_1_DIRECTION_GPIO_Port GPIOA
#define HALL_3_Pin GPIO_PIN_5
#define HALL_3_GPIO_Port GPIOA
#define HALL_2_Pin GPIO_PIN_0
#define HALL_2_GPIO_Port GPIOB
#define HALL_1_Pin GPIO_PIN_1
#define HALL_1_GPIO_Port GPIOB
#define SR_LATCH_Pin GPIO_PIN_12
#define SR_LATCH_GPIO_Port GPIOA
#define SR_CLOCK_Pin GPIO_PIN_15
#define SR_CLOCK_GPIO_Port GPIOA
#define SR_DATA_Pin GPIO_PIN_3
#define SR_DATA_GPIO_Port GPIOB
#define RELAY_Pin GPIO_PIN_4
#define RELAY_GPIO_Port GPIOB
#define GOMB_UP_Pin GPIO_PIN_6
#define GOMB_UP_GPIO_Port GPIOB
#define GOMB_UP_EXTI_IRQn EXTI9_5_IRQn
#define GOMB_DOWN_Pin GPIO_PIN_7
#define GOMB_DOWN_GPIO_Port GPIOB
#define GOMB_DOWN_EXTI_IRQn EXTI9_5_IRQn
#define GOMB_ENTER_Pin GPIO_PIN_8
#define GOMB_ENTER_GPIO_Port GPIOB
#define GOMB_ENTER_EXTI_IRQn EXTI9_5_IRQn
#define GOMB_MODE_Pin GPIO_PIN_9
#define GOMB_MODE_GPIO_Port GPIOB
#define GOMB_MODE_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
