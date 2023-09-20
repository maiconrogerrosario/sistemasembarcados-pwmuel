/***************************************************************************//**
 * Declare function prototype
 ******************************************************************************/
void inicializacoes(void);
void GPIO_Inits(void);
void Clock_Sys_Inits(void);
void Timer2_Inits(void);
void Interruption_Inits(void);
void SysTick_Configuration(void);
void Timer4_PWM_Inits(void);

/* Public variables ---------------------------------------------------------*/
extern s32 dutyClycleChannel1;
extern s32 dutyClycleChannel2;
extern s32 dutyClycleChannel3;
extern s32 dutyClycleChannel4;
extern s32 varTIM_Period;
