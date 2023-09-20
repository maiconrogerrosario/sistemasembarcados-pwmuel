/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "stm32f4xx_exti.h"
#include "Inicializacoes.h"
#include "LoopPrincipal.h"

/***************************************************************************//**
 * typedef unions and structures
 ******************************************************************************/
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;

/* Private variables ---------------------------------------------------------*/
vs32 TimingDelay;
s32 toggleLED = 0;

/* Timer 2 interrupt routine */
void TIM2_IRQHandler(void)
{
	//if interrupt happens then do this
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //clear interrupt flag

		/* Toggle LED4 */
//		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
	}
}

/**
 * @brief  This function handles External line 0 interrupt request.
 * @param  None
 * @retval None
 */
void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		/* Clear the EXTI line 0 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line0);

	}
}

/**
 * @brief  This function handles External line 2 interrupt request.
 * @param  None
 * @retval None
 */
void EXTI2_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		/* Clear the EXTI line 2 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line2);

		/* Toggle LED1 */
//		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);

	}
}

/**
 * @brief  This function handles External line 3 interrupt request.
 * @param  None
 * @retval None
 */
void EXTI3_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		/* Clear the EXTI line 2 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line3);

	}
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 * Setup SysTick Timer for 1 msec interrupts  */
void SysTick_Handler(void)
{
	if (TimingDelay != 0)
		TimingDelay--;

//	if (toggleLED <= 0)
//	{
//		toggleLED = 300;
//		/* Toggle LED3 */
//		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
//	}
//	toggleLED--;

}

