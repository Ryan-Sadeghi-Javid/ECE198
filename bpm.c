#include "stm32f4xx.h"

void GPIO_Init(void);
void turnOnGreenLED(void);
void turnOnYellowLED(void);
void turnOnRedLED(void);

int main(void) {
    HAL_Init();
    GPIO_Init();

    while (1) {
        // Simulated variable value (replace with your actual variable)
        uint32_t variableValue = 75; // Example value

        if (variableValue >= 40 && variableValue <= 90) {
            turnOnGreenLED();
        } else if ((variableValue >= 30 && variableValue <= 40) || (variableValue >= 100 && variableValue <= 120)) {
            turnOnYellowLED();
        } else {
            turnOnRedLED();
        }
    }
}

void GPIO_Init(void) { // Controls all of the input of the LED (General Purpose Input Output)
    // Enable the GPIO clock for port D
    __HAL_RCC_GPIOD_CLK_ENABLE();

    // Configure the GPIO pins for the LEDs
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void turnOnGreenLED(void) {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET); // Green LED on
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET); // Yellow LED off
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET); // Red LED off
}

void turnOnYellowLED(void) {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET); // Green LED off
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET); // Yellow LED on
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET); // Red LED off
}

void turnOnRedLED(void) {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET); // Green LED off
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET); // Yellow LED off
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET); // Red LED on
}
