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

#include "buttons.h"

struct sButton volatile buttons[M_BUTTON] = {0};

volatile uint8_t choice = false;

/**
 * A wrapper for an interrupt that polls buttons
 * @param button a number of button
 * @param idr IDR register of button
 * @param *func pointer to button handler
 */
void buttonTest(enum BUTTONS button, uint32_t idr, void (*func)(void)) {

    if ((!buttons[button].flag) || (buttons[button].count > BUTTON_DELAY - buttons[button].speedConst)) {
    buttons[button].count = 0;
    if ((BUTTON_PORT->IDR & idr) == 0) {
      buttons[button].flag = true;

      if (buttons[button].speedCount >= COUNT_EDGE)
        buttons[button].speedConst = BUTTON_SPEED_DELAY;
      
      buttons[button].speedCount++;

      if (backlight.flag) {
        backlight.count = 0;

 //       if (menu.position_choice > 0)
          func();
    
      } else
        StartBackLight();

    } else {
      buttons[button].speedCount = 0;
      buttons[button].speedConst = 0;
      buttons[button].flag = false;
    }
  } else
    buttons[button].count++;
  
}

/**
 * Polling of buttons
 */
void TIM14_IRQHandler(void)
{
  
  buttonTest(S2_BUTTON, S2_BUTTON_IDR, moveUp);
  buttonTest(S3_BUTTON, S3_BUTTON_IDR, valueDec);
  buttonTest(S4_BUTTON, S4_BUTTON_IDR, valueInc);
  buttonTest(S5_BUTTON, S5_BUTTON_IDR, moveDown);
  
/*  
  if ((!buttons[S2_BUTTON].flag) || (buttons[S2_BUTTON].count > BUTTON_DELAY - buttons[S2_BUTTON].speedConst)) {
    buttons[S2_BUTTON].count = 0;
    if ((BUTTON_PORT->IDR & S2_BUTTON_IDR) == 0) {
      buttons[S2_BUTTON].flag = true;

      if (buttons[S2_BUTTON].speedCount >= COUNT_EDGE)
        buttons[S2_BUTTON].speedConst = BUTTON_SPEED_DELAY;
      
      buttons[S2_BUTTON].speedCount++;

      if (backlight.flag) {
        backlight.count = 0;

 //       if (menu.position_choice > 0)
          moveUp();
    
      } else
        StartBackLight();

    } else {
      buttons[S2_BUTTON].speedCount = 0;
      buttons[S2_BUTTON].speedConst = 0;
      buttons[S2_BUTTON].flag = false;
    }
  } else
    buttons[S2_BUTTON].count++;

  if ((!buttons[S3_BUTTON].flag) || (buttons[S3_BUTTON].count > BUTTON_DELAY - buttons[S3_BUTTON].speedConst)) {
    buttons[S3_BUTTON].count = 0;
    if ((BUTTON_PORT->IDR & S3_BUTTON_IDR) == 0) {
      buttons[S3_BUTTON].flag = true;

      if (buttons[S3_BUTTON].speedCount >= COUNT_EDGE)
        buttons[S3_BUTTON].speedConst = BUTTON_SPEED_DELAY;
      
      buttons[S3_BUTTON].speedCount++;

      if (backlight.flag) {
        backlight.count = 0;

//        if (menu.position_choice > 0)
          valueDec();
    
      } else
        StartBackLight();

    } else {
      buttons[S3_BUTTON].speedCount = 0;
      buttons[S3_BUTTON].speedConst = 0;
      buttons[S3_BUTTON].flag = false;
    }
  } else
    buttons[S3_BUTTON].count++;
  
  if ((!buttons[S4_BUTTON].flag) || (buttons[S4_BUTTON].count > BUTTON_DELAY - buttons[S4_BUTTON].speedConst)) {
    buttons[S4_BUTTON].count = 0;
    if ((BUTTON_PORT->IDR & S4_BUTTON_IDR) == 0) {
      buttons[S4_BUTTON].flag = true;

      if (buttons[S4_BUTTON].speedCount >= COUNT_EDGE)
        buttons[S4_BUTTON].speedConst = BUTTON_SPEED_DELAY;
      
      buttons[S4_BUTTON].speedCount++;

      if (backlight.flag) {
        backlight.count = 0;

//        if (menu.position_choice > 0)
          valueInc();
    
      } else
        StartBackLight();

    } else {
      buttons[S4_BUTTON].speedCount = 0;
      buttons[S4_BUTTON].speedConst = 0;
      buttons[S4_BUTTON].flag = false;
    }
  } else
    buttons[S4_BUTTON].count++;
  
  
  if ((!buttons[S5_BUTTON].flag) || (buttons[S5_BUTTON].count > BUTTON_DELAY - buttons[S5_BUTTON].speedConst)) {
    buttons[S5_BUTTON].count = 0;
    if ((BUTTON_PORT->IDR & S5_BUTTON_IDR) == 0) {
      buttons[S5_BUTTON].flag = true;
         
      if (buttons[S5_BUTTON].speedCount >= COUNT_EDGE)
        buttons[S5_BUTTON].speedConst = BUTTON_SPEED_DELAY;
      
      buttons[S5_BUTTON].speedCount++;
   
      if (backlight.flag) {
        backlight.count = 0;
        
//        if (menu.position_choice < ITEMS_MAX - 1)
          moveDown();
  
      } else
        StartBackLight();
  
    } else {
      buttons[S5_BUTTON].speedCount = 0;
      buttons[S5_BUTTON].speedConst = 0;
      buttons[S5_BUTTON].flag = false;
    }
  } else
    buttons[S5_BUTTON].count++;
  */
  
  TIM14->SR &=~ TIM_SR_UIF;
}
