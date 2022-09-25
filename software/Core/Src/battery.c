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

#include "battery.h"

volatile unsigned char count = 0;
volatile bool batteryFlag = false;

#define MAX_AVERAGE    5

volatile unsigned int averageArray[MAX_AVERAGE] = {0};

/**
 * Interrupt for battery level calculatinon
 */
void ADC1_IRQHandler(void)
{
  
//  ADC1->DR

  if (ADC1->ISR & ADC_ISR_EOC) {            /* ADC1 EOC interrupt?                */
//    GPIOB->ODR ^= GPIO_ODR_7;
//    lcd8544_clear();
//    lcd8544_dec(ADC1->DR, 4, 3, 3, 0);
//    lcd8544_refresh();
    
    averageArray[count] = ADC1->DR;
    
    if (count < MAX_AVERAGE)
      count++;
    else {
      batteryFlag = true;
      count = 0;
    }
    
    ADC1->ISR |= ADC_ISR_EOC;
  }
    
//  ADC1->IER &= ~ADC_IER_EOCIE;
}

/**
 * Shaping display buffer with battery level
 */
void batteryShapping() {

  //lcd8544_rect(1, 0, 82, 5, 1);
  if (batteryFlag == 0)
    return;
  
  lcd8544_line(4, 0, 81, 0, 1);
  lcd8544_line(4, 5, 81, 5, 1);
  lcd8544_line(4, 0, 4, 5, 1);
  
  lcd8544_putpix(81, 1, 1);
  lcd8544_putpix(81, 4, 1);
  
  lcd8544_putpix(82, 2, 1);
  lcd8544_putpix(82, 3, 1);
  
//  ADC1->DR
  // R3 = 27k
  // R4 = 47k
  // max 4 V
  // min 3 V
  // 1 / 5 = 0.2
  // 3.2 = , 3.4 = , 3.6 = , 3.8 = , 4.0 = 
  // 12 bit ADC
  // 4 096
  
  // wait
  // 3.3 V + dropout
  // dropout is 200 mV
  // 3.5 V
  // 4 - 3.5 = 0.5
  // 0.5 / 5 = 0.1
  // 4.0 = 2.541,
  // 2.541 * 4096 / 3.3 = 3 153
  // 3.9V = 2.477V = 3 074ADC, 3.8V = 2.414V = 2 996ADC,
  // 3.7V = 2.35V = 2 916ADC, 3.6V = 2.286V = 2 837ADC, 3.5V = 2.223V = 2 759ADC
  // 
  
  unsigned int average = 0;
  for(int i = 0; i < MAX_AVERAGE; i++)
    average += averageArray[i];
  average /= MAX_AVERAGE;
  
  int division = 0;
  
  if (average >= 3153)
    division = 5;
  else if (average >= 3074)
    division = 4;
  else if (average >= 2996)
    division = 3;
  else if (average >= 2916)
    division = 2;
  else if (average >= 2837)
    division = 1;
  else
    division = 0;
  
  for(int i = 0; i < division; i++) {
    lcd8544_rect(6 + i * 15, 2, 19 + i * 15, 3, 1);
  }
  
  // checking iteration
  // 0:
  // 3 + 0 * 15 = 3
  // 18 + 0 * 15 = 18
  // 1:
  // 3 + 1 * 15 = 18
  // 18 + 1 * 15 = 33
  
  // 4:
  // 3 + 4 * 15 = 63
  // 16 + 4 * 15 = 76
  
  // 83 - 76 = 7
  // 7 / 2 = 3.5 | 4 - 3
 
/*  
  lcd8544_rect(4, 2, 18, 3, 1);
  lcd8544_rect(20, 2, 34, 3, 1);
  lcd8544_rect(36, 2, 50, 3, 1);
  lcd8544_rect(52, 2, 64, 3, 1);
  lcd8544_rect(66, 2, 81, 3, 1);
*/
//  lcd8544_refresh();
}