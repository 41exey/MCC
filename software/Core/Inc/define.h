/*
 * Copyright 2017 c1ewd
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

#ifndef DEFINE_H
#define DEFINE_H

#include "stm32f030x6.h"

#ifndef  __cplusplus
#define bool  unsigned char
#define true  1
#define false 0
#endif

#define NULL    (void*)0

#define LED_ODR GPIO_ODR_7
#define LED_PORT        GPIOB

#define MEMORY_PAGE     (uint32_t)0x08007000

#define BUZZER_ODR      GPIO_ODR_8
#define BUZZER_BRR      GPIO_BRR_BR_8
#define BUZZER_PORT     GPIOA

#endif