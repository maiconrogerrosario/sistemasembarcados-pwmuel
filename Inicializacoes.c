/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "Inicializacoes.h"

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
RCC_ClocksTypeDef RCC_ClockFreq;
NVIC_InitTypeDef NVIC_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;

/* Private variables ---------------------------------------------------------*/
s32 dutyClycleChannel1 = 0;
s32 dutyClycleChannel2 = 0;
s32 dutyClycleChannel3 = 0;
s32 dutyClycleChannel4 = 0;
s32 varTIM_Period = 0;

void inicializacoes(void)
{
	Clock_Sys_Inits();
	GPIO_Inits();
	Timer2_Inits();
	Timer4_PWM_Inits();
	Interruption_Inits();
	SysTick_Configuration();

}

void Clock_Sys_Inits(void)
{
	SystemInit();
}

void GPIO_Inits(void)
{
	/* GPIOD Periph clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);

	/* Configure PD12, PD13, PD14 in Alternate Function mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Configure PD15 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Alternating functions for pins PD12, PD13, PD14 ==> connect to TIM4 PWM OUTPUT*/
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
//    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

	/* Configure PB1 output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure PA0 input mode no pull up or pull down */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure PB2 e PB3 input mode internal pull up */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Timer2_Inits(void)
{
	// habilito o Clock do Timer 2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// Fills each TIM_TimeBaseInitStruct member with its default value.
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

//  uint16_t TIM_Prescaler;         /*!< Specifies the prescaler value used to divide the TIM clock.
//                                       This parameter can be a number between 0x0000 and 0xFFFF */
//
//  uint16_t TIM_CounterMode;       /*!< Specifies the counter mode.
//                                       This parameter can be a value of @ref TIM_Counter_Mode */
//
//  uint16_t TIM_Period;            /*!< Specifies the period value to be loaded into the active
//                                       Auto-Reload Register at the next update event.
//                                       This parameter must be a number between 0x0000 and 0xFFFF.  */
//
//  uint16_t TIM_ClockDivision;     /*!< Specifies the clock division.
//                                      This parameter can be a value of @ref TIM_Clock_Division_CKD */
//
//  uint8_t TIM_RepetitionCounter;  /*!< Specifies the repetition counter value. Each time the RCR downcounter
//                                       reaches zero, an update event is generated and counting restarts
//                                       from the RCR value (N).
//                                       This means in PWM mode that (N+1) corresponds to:
//                                          - the number of PWM periods in edge-aligned mode
//                                          - the number of half PWM period in center-aligned mode
//                                       This parameter must be a number between 0x00 and 0xFF.
//                                       @note This parameter is valid only for TIM1 and TIM8. */

	// T = (1/freq_bar) * (TIM_Prescaler - 1) * (TIM_Period - 1)
	//setting timer 2 interrupt to 2hz (84 MHz / 3600 / 11666 = 2hz --> T=1/2s)
	TIM_TimeBaseStructure.TIM_Prescaler = 3600 - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 11666 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Enables or disables the specified TIM interrupts. */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);
}

void Timer4_PWM_Inits(void)
{
	/*
	 * Alternate functions
	 * PD12 --> TIMR4 Channel 1 PWM output
	 * PD13 --> TIMR4 Channel 2 PWM output
	 * PD14 --> TIMR4 Channel 3 PWM output
	 *
	 * */

	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	// Fills each TIM_TimeBaseInitStruct member with its default value.
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	/*
	 * TIM4 is connected to APB1 bus --> works at 84MHz by default
	 * Timer count frequency is set with:
	 * timer_tick_frequency = Timer_default_frequency / (TIM_Prescaler + 1)
	 * doing TIM_Prescaler = 1 then
	 * timer_tick_frequency = 84000000 / (1 + 1) = 42000000
	 *
	 * Timer 4 is 16 bits ==> it counts from 0 to 65535 (maximum value for TIM_Prescaler and TIM_Period)
	 *
	 * PWM_frequency = timer_tick_frequency / (TIM_Period + 1)
	 * or TIM_Period = (timer_tick_frequency / PWM_frequency) - 1
	 * if PWM_frequency = 1 kHz
	 * TIM_Period = (42000000 / 1000) - 1 = 41999
	 *
	 * */
	 
	 
	// T = (1/freq_bar) * (TIM_Prescaler - 1) * (TIM_Period - 1)
	// TIM_Prescaler = 2 - 1 = 1
	// TIM_Period = 42000 - 1 = 41999
	// PWM_frequency = 1 kHz

	/* public Timer Period variable */
	varTIM_Period = 41999;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = varTIM_Period;
	TIM_TimeBaseStructure.TIM_Prescaler = 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/*
	 * PWM Mode configuration
	 * Duty Cycle equation:
	 * pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1
	 * where DutyCycle is in percent, between 0 and 100%
	 *
	 * Exemples:
	 * 5% duty cycle:     	TIM_Pulse = (((41999 + 1) * 5) / 100) - 1 = 2099
	 * 20% duty cycle:     	TIM_Pulse = (((41999 + 1) * 20) / 100) - 1 = 8399
	 * 75% duty cycle:     	TIM_Pulse = (((41999 + 1) * 75) / 100) - 1 = 31499
	 * 100% duty cycle:    	TIM_Pulse = (((41999 + 1) * 100) / 100) - 1 = 41999
	 *
	 * Remember: if (pulse_length > TIM_Period) --> output HIGH all the time
	 *
	 * */

	/* PWM Duty Cycle variables set */
	dutyClycleChannel1 = 2099; /* 5% duty cycle  */
	dutyClycleChannel2 = 8399; /* 20% duty cycle  */
	dutyClycleChannel3 = 31499; /* 75% duty cycle  */
	dutyClycleChannel4 = 419; /* 1% duty cycle  */

	/* PWM Mode configuration */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	/* PWM Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_Pulse = dutyClycleChannel1; /* 5% duty cycle  */
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* PWM Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_Pulse = dutyClycleChannel2; /* 20% duty cycle  */
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* PWM Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_Pulse = dutyClycleChannel3; /* 75% duty cycle  */
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	// vou usar somente os canais 1, 2 e 3 do TIM4
	/* PWM Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_Pulse = dutyClycleChannel4;  /* 75% duty cycle  */
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* TIM3 enable counter */
	TIM_Cmd(TIM4, ENABLE);
}

void Interruption_Inits(void)
{
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Connect EXTI Line0 to PA0 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	/* Connect EXTI Line2 to PB2 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);

	/* Connect EXTI Line3 to PB3 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource3);

	/* Configure EXTI Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Configure EXTI Line2 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Configure EXTI Line3 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable and set EXTI Line2 Interrupt priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable and set EXTI Line2 Interrupt priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable Timer2 IRQ */
	NVIC_ClearPendingIRQ(TIM2_IRQn);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Configure the SysTick Handler Priority: Preemption priority and subpriority */
	/* Set SysTick Priority to 3 */
	NVIC_SetPriority(SysTick_IRQn, 3);
}

void SysTick_Configuration(void)
{
	/* Setup SysTick Timer for 1 msec interrupts  */
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		/* Capture error */
		while (1)
			;
	}
}
