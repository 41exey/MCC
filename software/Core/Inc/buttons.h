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

#ifndef BUTTONS_H
#define BUTTONS_H

#include "stm32f030x6.h"
#include "backlight.h" 
#include "display.h"
#include "define.h"

void TIM14_IRQHandler(void);
/*
#define BUTTON_DELAY    (uint32_t)260
#define BUTTON_SPEED_DELAY    (uint32_t)100
#define COUNT_EDGE      (uint32_t)12
*/

#define BUTTON_DELAY    (uint32_t)360
#define BUTTON_SPEED_DELAY    (uint32_t)250
#define COUNT_EDGE      (uint32_t)6

/**
 * @struct sButton
 * Buttons polling struct
 */ 
struct sButton {
  /// count of sample
  unsigned int count;
  /// indicates button state
  bool flag;
  /// count of sample until speed increment
  unsigned char speedCount;
  /// pressing speed
  unsigned char speedConst;
};

#define BUTTON_PORT     GPIOA
#define S2_BUTTON_IDR   GPIO_IDR_1
#define S3_BUTTON_IDR   GPIO_IDR_2
#define S4_BUTTON_IDR   GPIO_IDR_3
#define S5_BUTTON_IDR   GPIO_IDR_4

enum BUTTONS {
  S2_BUTTON,
  S3_BUTTON,
  S4_BUTTON,
  S5_BUTTON,
  M_BUTTON
};

void buttonTest(enum BUTTONS button, uint32_t idr, void (*func)(void));

#endif