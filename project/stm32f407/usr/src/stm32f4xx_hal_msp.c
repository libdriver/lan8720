/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      stm32f4xx_hal_msp.c
 * @brief     stm32f4xx hal msp source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-11-11
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/11/11  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "stm32f4xx_hal.h"
#include "eth.h"

/** 
 * @defgroup msp HAL_MSP_Private_Functions
 * @{
 */

/**
 * @brief  initializes the global msp.
 * @note   none
 */
void HAL_MspInit(void)
{
    
}

/**
 * @brief  deinitializes the global msp.
 * @note   none
 */
void HAL_MspDeInit(void)
{
    
}

/**
 * @brief     uart hal init
 * @param[in] *huart points to a uart handle
 * @note      none
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    if (huart->Instance == USART1)
    {
        /* enable uart gpio clock */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        
        /* enable usart1 clock */
        __HAL_RCC_USART1_CLK_ENABLE();
        
        /**
         * PA9  ------> USART1_TX
         * PA10 ------> USART1_RX 
         */
        GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        /* enable nvic */
        HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
    if (huart->Instance == USART2)
    {
        /* enable uart gpio clock */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        
        /* enable usart2 clock */
        __HAL_RCC_USART2_CLK_ENABLE();
    
        /**
         * PA2 ------> USART2_TX
         * PA3 ------> USART2_RX 
         */
        GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        /* enable nvic */
        HAL_NVIC_SetPriority(USART2_IRQn, 2, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
}

/**
 * @brief     uart hal deinit
 * @param[in] *huart points to a uart handle
 * @note      none
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
    if (huart->Instance == USART1)
    {
        /* disable usart1 clock */
        __HAL_RCC_USART1_CLK_DISABLE();
        
        /* uart gpio deinit */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
        
        /* disable nvic */
        HAL_NVIC_DisableIRQ(USART1_IRQn);
    }
    if (huart->Instance == USART2)
    {
        /* disable usart2 clock */
        __HAL_RCC_USART2_CLK_DISABLE();
        
        /* uart gpio deinit */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);
        
        /* disable nvic */
        HAL_NVIC_DisableIRQ(USART2_IRQn);
    }
}

/**
 * @brief     eth hal init
 * @param[in] *heth points to an eth handle
 * @note      none
 */
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    /* enable eth gpio clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    
    /* enable eth clock */
    __HAL_RCC_ETH_CLK_ENABLE();
    
    /**
     * ETH_MDIO -------------------------> PA2
     * ETH_RMII_REF_CLK -----------------> PA1
     * ETH_RMII_CRS_DV ------------------> PA7
     */
    GPIO_Initure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Pull = GPIO_NOPULL;
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    GPIO_Initure.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOA, &GPIO_Initure); 
    
    /**
     * ETH_MDC --------------------------> PC1
     * ETH_RMII_RXD0 --------------------> PC4
     * ETH_RMII_RXD1 --------------------> PC5
     */
    GPIO_Initure.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);
    
    /**
     * ETH_RMII_TX_EN -------------------> PG11
     * ETH_RMII_TXD0 --------------------> PG13
     * ETH_RMII_TXD1 --------------------> PG14
     */
    GPIO_Initure.Pin = GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14;
    HAL_GPIO_Init(GPIOG, &GPIO_Initure);
    
    /**
     * ETH_RESET ------------------------> PD3
     */
    GPIO_Initure.Pin = GPIO_PIN_3;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_Initure);
    
    /* enable nvic */
    HAL_NVIC_SetPriority(ETH_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(ETH_IRQn);
    
    /* set mdio clock range */
    HAL_ETH_SetMDIOClockRange(eth_get_handle());
}

/**
 * @brief     eth hal deinit
 * @param[in] *heth points to an eth handle
 * @note      none
 */
void HAL_ETH_MspDeInit(ETH_HandleTypeDef *heth)
{
    /* disable eth clock */
    __HAL_RCC_ETH_CLK_DISABLE();
    
    /* gpio deinit */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7);
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5);
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_3);
    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14);
    
    /* disable nvic */
    HAL_NVIC_DisableIRQ(ETH_IRQn);
}

/**
 * @}
 */
