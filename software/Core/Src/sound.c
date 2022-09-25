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

#include "sound.h"
#include "stm32f030x6.h"
#include "define.h"
#include "flash.h"
#include "am2302.h"

volatile unsigned char soundDelay;

volatile unsigned char seconds = 0;
volatile unsigned char step = 0;

/**
 * Sound signal
 */ 
unsigned char turn[MAX_STEP] = {ON, OFF, ON, OFF, ON, OFF, OFF, OFF, OFF};

void sound(unsigned char value);

/**
 * Internal time for sound signal
 */ 
void TIM16_IRQHandler(void) {


  if (soundDelay > 0) {
  
    if (seconds >= 60) {
      seconds = 0;
      soundDelay--;
    }
  
    seconds++;
    step = 0;
  } else {
    
    seconds = 0;
    
    if (sensor.temperature > settings.temperatureHight \
      || sensor.temperature < settings.temperatureLow \
      || sensor.humidity > settings.humidityHight \
      || sensor.humidity < settings.humidityLow) {
        
        if (step >= MAX_STEP)
          step = 0;
        
        sound(turn[step]);
        //GPIOB->ODR ^= GPIO_ODR_7;
        LED_PORT->ODR ^= LED_ODR;
//        BUZZER_PORT->ODR ^= BUZZER_ODR;
        
        step++;
      } else
        step = 0;

  }

  TIM16->SR &=~ TIM_SR_UIF;
}

/**
 * Get sound signal
 * @param value activate speaker
 */ 
void sound(unsigned char value) {

  switch(value) {
  case OFF:
    BUZZER_PORT->BRR |= BUZZER_BRR;
    break;
  case ON:
    BUZZER_PORT->BSRR |= BUZZER_BRR;
  }

 /*
  switch(value) {
  case OFF:
    BUZZER_PORT->ODR &= ~BUZZER_ODR;
    break;
  case ON:
    BUZZER_PORT->ODR |= BUZZER_ODR;
  }  
  */
}