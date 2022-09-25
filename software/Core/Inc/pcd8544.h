/*
 * Copyright 2015 Gauss
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

#ifndef PCD8544_H
#define PCD8544_H

#include "stm32f030x6.h"

// Pinout
#define SCLK    GPIO_BRR_BR_10
#define MOSI    GPIO_BRR_BR_9
#define DC      GPIO_BRR_BR_7
#define RST     GPIO_BRR_BR_5
#define SCE     GPIO_BRR_BR_6

#define LCD_PORT GPIOA	// Dispaly port

void lcd8544_clear (void);

void lcd8544_init(void);  // Display initialization

void lcd8544_power_down(void);

void lcd8544_power_up(void);

void lcd8544_refresh(void); // Refresh from buffer

void lcd8544_putpix(unsigned char x, unsigned char y, unsigned char mode); // Put pixel

void lcd8544_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char mode); // Show line
void lcd8544_rect(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char mode); // Show rectangle 
void lcd8544_putchar(unsigned char px, unsigned char py, unsigned char ch, unsigned char mode); // Show char

void lcd8544_dec(unsigned int numb, unsigned char dcount, unsigned char x, unsigned char y, unsigned char mode); // Show decimal number

void lcd8544_putstr(unsigned char x, unsigned char y, const unsigned char str[], unsigned char mode); // Show string

void lcd8544_putobject(unsigned char px,unsigned char py,unsigned char width,unsigned char height,unsigned char object[]); // Show graphical object

unsigned char *ftoa(float f, int dec); // Convert float number to string

#endif