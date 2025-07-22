#include "main.h"

#include "string.h"
#include <stdio.h>
#include <stdlib.h>

UART_HandleTypeDef huart2;
TIM_HandleTypeDef htim2;

char rxBuffer[2];
typedef struct {
    float angle;
    int direction;
    int rpm;
} Stepper;

Stepper motor;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);


void delay (uint16_t us){

	__HAL_TIM_SET_COUNTER(&htim2,0);
	while(__HAL_TIM_GET_COUNTER(&htim2)<us);
}


#define stepsperrev 4096

void stepper_set_rpm (int rpm)
{
	if(rpm<5){
     rpm=5;
	}
	if(rpm>240){
	     rpm=120;
		}

	delay(60000000 / stepsperrev / rpm);
}

void stepper_half_drive(uint8_t step)
{
	switch(step){
		case 0:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
			  break;
		case 1:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
			  break;
		case 2:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
			  break;
		case 3:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
			  break;
		case 4:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
			  break;
		case 5:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
			  break;
		case 6:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
			  break;
		case 7:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
			  break;
	}
}

void stepper_step_angle (float angle, int direction,int rpm)
{
	float anglepersequence = 0.703125;  // 360 = 512 sequences
	int numberofsequences = (int) (angle/anglepersequence);

	motor.angle = angle;
	motor.direction = direction;
	motor.rpm = rpm;

	for (int seq=0; seq<numberofsequences; seq++)
		{
			if (direction == 0)
			{
				  for(int j=7; j>=0; j--)
				  {
					  stepper_half_drive(j);
					  stepper_set_rpm(rpm);
				  }
			}
			if (direction == 1)  // not clockwise
			{
				  for(int j=0; j<8; j++)
				  {
					  stepper_half_drive(j);
					  stepper_set_rpm(rpm);
				  }
			}
		}
}

void stop_stepMotor(){
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
}

void read_buffer(){


	if(rxBuffer[0] == '4'){ // code  for direction

		if(rxBuffer[1] == '0'){ //clockwise
		stepper_step_angle(motor.angle,0,motor.rpm);
		}

		else if(rxBuffer[1] == '1'){
	    stepper_step_angle(motor.angle,1,motor.rpm);
		}

		else{
			Error_Handler();
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		}

	}
   else if(rxBuffer[0] == '1'){ // code for angle

	   if(rxBuffer[1] == '0'){ // 45
	   stepper_step_angle(45.0,motor.direction,motor.rpm);
	  }

	  else if(rxBuffer[1] == '1'){ // 90
		  stepper_step_angle(90.0,motor.direction,motor.rpm);
	  	}

	  else if(rxBuffer[1] == '2'){ // 135
		  stepper_step_angle(135.0,motor.direction,motor.rpm);
	  					   							   		}
	  else if(rxBuffer[1] == '3'){ // 180
		  stepper_step_angle(180.0,motor.direction,motor.rpm);
	  					   							   		}
	  else if(rxBuffer[1] == '4'){ // 270
		  stepper_step_angle(270.0,motor.direction,motor.rpm);
	  					   							   							   		}
	  else if(rxBuffer[1] == '5'){ // 360
		  stepper_step_angle(360.0,motor.direction,motor.rpm);}

	  else if(rxBuffer[1] == '6'){ // 720
	  	  stepper_step_angle(720.0,motor.direction,motor.rpm);
	  }
	  else{
	  Error_Handler();
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	  					   		}

   }

   else if(rxBuffer[0] == '2'){ // code for speed

	   if(rxBuffer[1] == '0'){ //x2 speed
			stepper_step_angle(motor.angle,motor.direction,motor.rpm*2);
	   		}

	   		else if(rxBuffer[1] == '1'){ // /2 speed
	   	    stepper_step_angle(motor.angle,motor.direction,motor.rpm/2);
	   		}

	   		else if(rxBuffer[1] == '2'){ // x3 speed
	   	    stepper_step_angle(motor.angle,motor.direction,motor.rpm*3);
	   							   		}
	   		else if(rxBuffer[1] == '3'){ // /3 speed
	   	    stepper_step_angle(motor.angle,motor.direction,motor.rpm/3);
	   							   		}
	   		else if(rxBuffer[1] == '4'){ // x5 speed
	   		stepper_step_angle(motor.angle,motor.direction,motor.rpm*5);
	   							   							   		}
	   		else if(rxBuffer[1] == '5'){ // /5 speed
	   	    stepper_step_angle(motor.angle,motor.direction,motor.rpm/5);}

	   		else{
	   		Error_Handler();
	   		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	   		}

   				  	 }

   else if(rxBuffer[0] == '3'){
	   stop_stepMotor();

   }
	 memset(rxBuffer, 0, sizeof(rxBuffer));
	 stop_stepMotor();
	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
}

int main(void)
{

  motor.angle = 45.0;
  motor.direction = 0;
  motor.rpm = 40;

  HAL_Init();


  SystemClock_Config();


  MX_GPIO_Init();
  MX_TIM2_Init();
  HAL_TIM_Base_Start(&htim2);
  MX_USART2_UART_Init();



   if(HAL_UART_Receive_IT(&huart2, (uint8_t *)rxBuffer, 2)!=HAL_OK){
 	  Error_Handler();
 	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
   }


  while (1)
  {

  }

}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};


  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);


  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }


  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_USART2_UART_Init(void)
{


  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}


static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();


  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);


  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  __HAL_RCC_GPIOH_CLK_ENABLE();

 	GPIO_InitTypeDef UartPinTanimla = {0};

 	UartPinTanimla.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
 	UartPinTanimla.Mode = GPIO_MODE_OUTPUT_PP;
 	UartPinTanimla.Pull = GPIO_NOPULL;
 	UartPinTanimla.Speed = GPIO_SPEED_FREQ_HIGH;

 	HAL_GPIO_Init(GPIOD, &UartPinTanimla);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);


 	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
 	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
 	GPIO_InitStruct.Pull = GPIO_NOPULL;
 	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
 	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


}

static void MX_TIM2_Init(void){

	TIM_ClockConfigTypeDef saatKaynakAyarla = {0};
	TIM_MasterConfigTypeDef masterAyarla = {0};

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 8400-1; // adds+1 84
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 65535-1;
	htim2.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if(HAL_TIM_Base_Init(&htim2)!=HAL_OK){

		Error_Handler();
	}

	saatKaynakAyarla.ClockSource =TIM_CLOCKSOURCE_INTERNAL;
	if(HAL_TIM_ConfigClockSource(&htim2,&saatKaynakAyarla)!=HAL_OK){

		Error_Handler();
	}

	masterAyarla.MasterOutputTrigger = TIM_TRGO_RESET;
	masterAyarla.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;


	if(HAL_TIMEx_MasterConfigSynchronization(&htim2,&masterAyarla)!=HAL_OK){

			Error_Handler();
	}

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	 read_buffer();

	 memset(rxBuffer, 0, sizeof(rxBuffer));

	 if(HAL_UART_Receive_IT(&huart2, (uint8_t *)rxBuffer, 2)!=HAL_OK){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		Error_Handler();
	 }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
     HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_12);
}


void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }

}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif

