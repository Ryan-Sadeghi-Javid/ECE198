#include "stm32f4xx_hal.h"

ADC_HandleTypeDef hadc1;

// Define GPIO pins for LEDs
#define GREEN_LED_PIN GPIO_PIN_1
#define YELLOW_LED_PIN GPIO_PIN_2
#define RED_LED_PIN GPIO_PIN_3

// Define GPIO port for LEDs
#define LED_PORT GPIOA

// Function prototypes
void SystemClock_Config(void);
void Error_Handler(void);

int main(void) {
    // Initialize the HAL Library
    HAL_Init();

    // Configure the system clock
    SystemClock_Config();

    // Initialize the GPIO peripheral
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GREEN_LED_PIN | YELLOW_LED_PIN | RED_LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    // Initialize ADC
    __HAL_RCC_ADC1_CLK_ENABLE();
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.NbrOfDiscConversion = 0;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK) {
        Error_Handler();
    }

    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }

    // Start ADC conversion
    if (HAL_ADC_Start(&hadc1) != HAL_OK) {
        Error_Handler();
    }

    while (1) {
        // Wait for the ADC conversion to complete
        if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
            // Read the ADC value

            uint16_t variableValue = HAL_ADC_GetValue(&hadc1)/1024;

            // Check conditions and control LEDs accordingly
            if (variableValue >= 40 && variableValue <= 90) {
                // Turn on the green LED
                HAL_GPIO_WritePin(LED_PORT, GREEN_LED_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LED_PORT, YELLOW_LED_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_PORT, RED_LED_PIN, GPIO_PIN_RESET);
            } else if ((variableValue >= 30 && variableValue <= 40) || (variableValue >= 100 && variableValue <= 120)) {
                // Turn on the yellow LED
                HAL_GPIO_WritePin(LED_PORT, GREEN_LED_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_PORT, YELLOW_LED_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LED_PORT, RED_LED_PIN, GPIO_PIN_RESET);
            } else {
                // Turn on the red LED
                HAL_GPIO_WritePin(LED_PORT, GREEN_LED_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_PORT, YELLOW_LED_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_PORT, RED_LED_PIN, GPIO_PIN_SET);
            }
        }
    }
}

// System Clock Configuration
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    // SysTick_IRQn interrupt configuration
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

// Error Handler
void Error_Handler(void) {
    // User can add his own implementation to report the HAL error return state
    while (1) {
        // Infinite loop
    }
}
