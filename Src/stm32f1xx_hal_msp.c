

#include "stm32f1xx_hal.h"
#include "timer.h"

void HAL_MspInit(void)
{
    /* Enable power and alternate function clocks */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();

    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
}

/* Low-level hardware initialization for the TIM2 peripheral */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {

	__HAL_RCC_TIM2_CLK_ENABLE();

	/* Set interrupt priority and enable IRQ in the NVIC */
	HAL_NVIC_SetPriority(TIM2_IRQn, TIM2_IRQn_Preempt_Priority, TIM2_IRQn_Sub_Priority);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

}
