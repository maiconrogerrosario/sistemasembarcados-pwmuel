/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "Inicializacoes.h"
#include "LoopPrincipal.h"
#include "ISR.h"

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Loop Principal
 * @param  None
 * @retval None
 */
void loopPrincipal(void)
{
	while (1)
	{
		u32 var;

		// incremento o DC de 0% a 100%
		for (var = 0; var < varTIM_Period; ++var)
		{
			/* change Duty Cycle TIMx Channel 1 */
			TIM_SetCompare1(TIM4, var);
			Delay(4000);
		}

		/* Toggle LED1 */
//		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == SET) GPIO_ToggleBits(GPIOD, GPIO_Pin_15);

		// decremento o DC de 100% a 0%
		for (var = varTIM_Period; var > 0; --var)
		{
			/* change Duty Cycle TIMx Channel 1 */
			TIM_SetCompare1(TIM4, var);
			Delay(4000);
		}

		/* Toggle LED1 */
//		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == SET) GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
	}
}

/**
 * @brief  Delay Function.
 * @param  nCount:specifies the Delay time length.
 * @retval None
 */
void Delay(__IO uint32_t nCount)
{
	while (nCount--)
	{
	}
}

/**
 * @brief  Delay Function in ms.
 * @param  miliSegundos: specifies the Delay time length in ms.
 * @retval None
 */
void delay_ms(u32 miliSegundos)
{
	TimingDelay = miliSegundos;
	while (TimingDelay)
		;
}

