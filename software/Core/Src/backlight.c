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

#include "backlight.h"

struct sBacklight volatile backlight = {0};

/**
 * Start backlight
 */
void StartBackLight() {
  
  lcd8544_power_up();
  show();
  TIM3->CCR3 = 100;
  TIM3->CR1 |= TIM_CR1_CEN;
  backlight.count = 0;
  backlight.flag = true;
  
}

/**
 * Backlight PWM
 */
void TIM3_IRQHandler(void)
{

  /*
  if (direction == DIRECTION_UP && TIM3->CCR3 < 100)
    TIM3->CCR3++;
  else if (direction == DIRECTION_UP && TIM3->CCR3 >= 100)
    direction = DIRECTION_DOWN;
  else if (direction == DIRECTION_DOWN && TIM3->CCR3 > 0)
    TIM3->CCR3--;
  else if (direction == DIRECTION_DOWN && TIM3->CCR3 <= 0)
    direction = DIRECTION_UP;
 */
  
  if ((backlight.count == 0) && (TIM3->CCR3 > 0))
    TIM3->CCR3--;
  else if (backlight.count < BACKLIGHT_DELAY)
    backlight.count++;
  else if ((backlight.count == BACKLIGHT_DELAY) && (TIM3->CCR3 < 100))
    TIM3->CCR3++;
  else {
    TIM3->CCR3 = 100;
    TIM3->CR1 &= ~TIM_CR1_CEN;
    lcd8544_power_down();
    backlight.flag = false;
    backlight.count = 0;
  }
  
  TIM3->SR &=~ TIM_SR_UIF;
}

/*
void TIM16_IRQHandler(void) 
{
  TIM16->SR &=~ TIM_SR_UIF;
}
*/