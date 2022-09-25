/*
 * Copyright 2020 c1ewd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "main.h"
#include "define.h"

#include "am2302.h"
#include "buttons.h"
#include "backlight.h"
#include "flash.h"
#include "string.h"
#include "sound.h"

/**
Test function indicates that MCU works
*/
void TIM17_IRQHandler(void)
{
//  LED_PORT->ODR ^= LED_ODR;
//  BUZZER_PORT->ODR ^= BUZZER_ODR;
    
  TIM17->SR &=~ TIM_SR_UIF;
}

/**
 * Program delay in milliseconds
 * @param ms delay in milliseconds
 */
void delay(uint32_t ms) {
  for(int i = 0; i < ms; i ++)
  {//48000000
    for(int j = 0; j < 48000; j ++)
    {
      __NOP();
    }
  }
}

int main(void)
{

  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  // led
  GPIOB->MODER |= GPIO_MODER_MODER7_0;
  GPIOB->OTYPER &= ~GPIO_OTYPER_OT_7;
  GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7;
  GPIOB->PUPDR |= GPIO_PUPDR_PUPDR7_0;
  
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  // Display
  // PA5
  GPIOA->MODER |= GPIO_MODER_MODER5_0;
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5;
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5;
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5;
  // PA6
  GPIOA->MODER |= GPIO_MODER_MODER6_0;
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_6;
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6;
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR6;   
  // PA7
  GPIOA->MODER |= GPIO_MODER_MODER7_0;
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_7;
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7;
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR7;     
  // PA9
  GPIOA->MODER |= GPIO_MODER_MODER9_0;
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_9;
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9;
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR9;
  // PA10
  GPIOA->MODER |= GPIO_MODER_MODER10_0;
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_10;
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10;
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR10; 

  // PWM Display
  // TIM3_CH3
  // Configure PB0 for BL of LCD
//  GPIOB->MODER &= ~GPIO_MODER_MODER0;
  GPIOB->MODER |= GPIO_MODER_MODER0_1;
  GPIOB->OTYPER &= ~GPIO_OTYPER_OT_0;
  GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0;
  GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR0;    
    
  // Enable PWM for LCD
  // Select AF1 on PB0 in AFRL for TIM3_CH3
  GPIOB->AFR[0] |= 0x01 << GPIO_AFRL_AFRL0_Pos;
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  TIM3->PSC = 4800 - 1;
  TIM3->ARR = 100;
  TIM3->CCR3 = 100;
  TIM3->CCER |= TIM_CCER_CC3E;
  TIM3->BDTR |= TIM_BDTR_MOE;
  TIM3->CCMR2 = TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;
  NVIC_SetPriority(TIM3_IRQn, 1);
  NVIC_EnableIRQ(TIM3_IRQn);
  TIM3->DIER = TIM_DIER_UIE;    
  TIM3->EGR |= TIM_EGR_UG;
    
  // time
  RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
  TIM16->PSC = 48000 - 1;
  TIM16->ARR = 500 - 1; // one per second, tested
  TIM16->CNT = 0;
  NVIC_SetPriority(TIM16_IRQn, 1);
  NVIC_EnableIRQ(TIM16_IRQn);
  TIM16->DIER = TIM_DIER_UIE;    
  TIM16->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE;
  
  // buzzer
  GPIOA->MODER |= GPIO_MODER_MODER8_0;
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_8;
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8;
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR8;
  
  // TIM for tests
  /*
  RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;
  TIM17->PSC = 48000 - 1;
  TIM17->ARR = 500;
  TIM17->CNT = 0;
  NVIC_SetPriority(TIM17_IRQn, 1);
  NVIC_EnableIRQ(TIM17_IRQn);
  TIM17->DIER = TIM_DIER_UIE;    
  TIM17->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE;
*/
  
  // PA0 ADC
  GPIOA->MODER |= GPIO_MODER_MODER0;
  // RCC of ADC
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  RCC->CR2 |= RCC_CR2_HSI14ON;
  while((RCC->CR2 & RCC_CR2_HSI14RDY) == 0) {

  }

  ADC1->CFGR2 &= ~ADC_CFGR2_CKMODE;
  ADC1->CFGR2 |= ADC_CFGR2_CKMODE_1;
    
  ADC1->CFGR1 = 0;
    
  ADC1->CFGR1 |= ADC_CFGR1_CONT | ADC_CFGR1_SCANDIR;
  ADC1->CHSELR = ADC_CHSELR_CHSEL0;
  ADC1->SMPR |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2;
  ADC1->IER = ADC_IER_EOCIE;
  NVIC_EnableIRQ(ADC1_COMP_IRQn);
  NVIC_SetPriority(ADC1_COMP_IRQn, 2);
    
  // Calibration
  // If enable
  if ((ADC1->CR & ADC_CR_ADEN) != 0) {
    ADC1->CR |= ADC_CR_ADDIS;
  }
  
  while((ADC1->CR & ADC_CR_ADEN) != 0) {

  }

  ADC1->CFGR1 &= ~ADC_CFGR1_DMAEN;
  ADC1->CR |= ADC_CR_ADCAL;
  while((ADC1->CR & ADC_CR_ADCAL) != 0) {
  }
    
  // ADC enable

  ADC1->CR |= ADC_CR_ADEN;
  while((ADC1->ISR & ADC_ISR_ADRDY) == 0) {
   
  }
    
  ADC1->CR |= ADC_CR_ADSTART;
  
  // AM2302
  // DATA pin is PB3
  GPIOB->MODER &= ~GPIO_MODER_MODER3;
  GPIOB->MODER |= GPIO_MODER_MODER3_0;
  GPIOB->OTYPER |= GPIO_OTYPER_OT_3;
  GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3;
  GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR3;
    
  // AM2302 polling by TIM1
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
  // 240 / 48 000 000 = 1 / 200 000 = 0.000005 s = 0.005 ms = 5 us
  TIM1->PSC = 240 - 1;
  TIM1->ARR = 1;
  NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 0);
  NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
  TIM1->DIER = TIM_DIER_UIE;    
  TIM1->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE;
  TIM1->EGR |= TIM_EGR_UG;

  // Buttons
  // S2 - PA1
  // S3 - PA2
  // S4 - PA3
  // S5 - PA4
  // PA1
  GPIOA->MODER &= ~GPIO_MODER_MODER1; // Input mode
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_1; // Output push pull
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR1; // High speed
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR1;
  GPIOA->PUPDR |= GPIO_PUPDR_PUPDR1_0; // Pull-up
  // PA2
  GPIOA->MODER &= ~GPIO_MODER_MODER2; // Input mode
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_2; // Output push pull
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2; // High speed
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR2;
  GPIOA->PUPDR |= GPIO_PUPDR_PUPDR2_0; // Pull-up    
  // PA3
  GPIOA->MODER &= ~GPIO_MODER_MODER3; // Input mode
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_3; // Output push pull
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3; // High speed
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR3;
  GPIOA->PUPDR |= GPIO_PUPDR_PUPDR3_0; // Pull-up
  // PA4
  GPIOA->MODER &= ~GPIO_MODER_MODER4; // Input mode
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_4; // Output push pull
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR4; // High speed
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR4;
  GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_0; // Pull-up
    
  // Buttons polling by TIM14
  RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
  TIM14->PSC = 480 - 1;
  TIM14->ARR = 50;
  TIM14->CNT = 0;
  NVIC_SetPriority(TIM14_IRQn, 1);
  NVIC_EnableIRQ(TIM14_IRQn);
  TIM14->DIER = TIM_DIER_UIE;    
  TIM14->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE;
  
  lcd8544_init();
  lcd8544_clear();
  lcd8544_refresh(); 
  lcd8544_power_down();
  
  soundDelay = 10;
  
//  flashFirst();
  
  flashRead((unsigned char *)&settings, MEMORY_PAGE, sizeof(settings)); // page 28
//  memcpy((void *)&settingsBackup, (void *)&settings, sizeof(settings)); // wait for changes
  
  while(1) {

    sensorRead();
    show();
    delay(100);
    
  }
  
}
