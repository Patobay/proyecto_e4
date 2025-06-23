
#include "main.h"
#include "stm32f1xx_hal.h"
#include <stdio.h>

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart1;

uint16_t leer_luz(uint8_t canal) {
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = canal;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    uint16_t valor = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return valor;
}

#define UMBRAL 3000

void controlar_leds() {
    uint16_t valores[4];
    for (int i = 0; i < 4; i++) {
        valores[i] = leer_luz(ADC_CHANNEL_0 + i);
    }

    for (int i = 0; i < 4; i++) {
        if (valores[i] < UMBRAL) {
            for (int j = 0; j < 4; j++) {
                GPIO_InitTypeDef GPIO_InitStruct = {0};
                GPIO_InitStruct.Pin = (GPIO_PIN_0 << j);
                if (j != i) {
                    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
                    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
                    HAL_GPIO_WritePin(GPIOA, (GPIO_PIN_0 << j), GPIO_PIN_SET);
                } else {
                    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
                    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
                }
            }
        }
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_USART1_UART_Init();

    while (1) {
        controlar_leds();
        HAL_Delay(200);
    }
}
