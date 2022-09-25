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

#include "display.h"
#include "flash.h"
//#include "string.h"
#include "sound.h"
//unsigned char volatile soundDelay;

struct sMenu menu[3] = {{0,
                    {{"Темп-ра", TEMPERATURE_ITEM, (void*)&sensor.temperature},
                    {" Нижний", TEMPERATURE_LOW_ITEM, (void*)&settings.temperatureLow},
                    {" Верхний", TEMPERATURE_HIGHT_ITEM, (void*)&settings.temperatureHight},
                    {"Влаж-ть", HUMIDITY_ITEM, (void*)&sensor.humidity},
                    {" Нижний", HUMIDITY_LOW_ITEM, (void*)&settings.humidityLow},
                    {" Верхний", HUMIDITY_HIGHT_ITEM, (void*)&settings.humidityHight},
                    {"Зад-ка з.", SOUND_DELAY_ITEM, (void*)&soundDelay},
                    {"СОХРАНИТЬ", SAVE_ITEM, NULL}},
                    0},
                    {0,
                    {{"НЕТ", NO_ITEM, NULL},
                    {"ДА", YES_ITEM, NULL}},
                    0},
                    {0,
                    {{"СОХРАНЕНО!", SAVED_ITEM, NULL}},
                    0}};

/// \cond

/**
 *  The display pixels makes with help display buffer. Buffer is placing in pcd8544 driver files. Shaping
 * functions acts as a wrapper between the display and the periphery. They form the display buffer.
 */

/// \endcond

/**
 * Forms lines with parameters value. Show visible values only
 */
void listShapping(void) {
      
  if (menu[LIST_MENU].items[0].id == TEMPERATURE_ITEM && menu[LIST_MENU].position_choice == 0) {
    menu[LIST_MENU].position_choice++;
  }
      
  for(int i = menu[LIST_MENU].position_view, j = 0; i < menu[LIST_MENU].position_view + ITEMS_TOGETHER; i++, j++) {
    if (i == menu[LIST_MENU].position_choice) {

//      if ((menu[LIST_MENU].items[i].id == SAVE_ITEM) && (memcmp((void *)&settingsBackup, (void *)&settings, sizeof(settings)) == 0))
      if (menu[LIST_MENU].items[i].id == SAVE_ITEM)
        lcd8544_putstr(15, j * LINE_HEIGHT + TOP_SHIFT, menu[LIST_MENU].items[i].text, 1);
      else if (menu[LIST_MENU].items[i].id != SAVE_ITEM)
        lcd8544_putstr(0, j * LINE_HEIGHT + TOP_SHIFT, menu[LIST_MENU].items[i].text, 1);

      switch(i) {
      case TEMPERATURE_ITEM:
        lcd8544_putstr(60, j * LINE_HEIGHT + TOP_SHIFT, ftoa(sensor.temperature, 1), 0);
        break;
      case TEMPERATURE_LOW_ITEM:
        lcd8544_putstr(60, j * LINE_HEIGHT + TOP_SHIFT, ftoa(settings.temperatureLow, 1), 0);
        break;
      case TEMPERATURE_HIGHT_ITEM:
        lcd8544_putstr(60, j * LINE_HEIGHT + TOP_SHIFT, ftoa(settings.temperatureHight, 1), 0);
        break;
      case HUMIDITY_ITEM:
        lcd8544_putstr(60, j * LINE_HEIGHT + TOP_SHIFT, ftoa(sensor.humidity, 1), 0);
        break;
      case HUMIDITY_LOW_ITEM:
        lcd8544_putstr(60, j * LINE_HEIGHT + TOP_SHIFT, ftoa(settings.humidityLow, 1), 0);
        break;
      case HUMIDITY_HIGHT_ITEM:
        lcd8544_putstr(60, j * LINE_HEIGHT + TOP_SHIFT, ftoa(settings.humidityHight, 1), 0);
        break;
      case SOUND_DELAY_ITEM:
        lcd8544_dec(soundDelay, 3, 60, j * LINE_HEIGHT + TOP_SHIFT, 0);
        break;
      }
    } else
      
//      if ((menu[LIST_MENU].items[i].id == SAVE_ITEM) && (memcmp((void *)&settingsBackup, (void *)&settings, sizeof(settings)) == 0))
      if (menu[LIST_MENU].items[i].id == SAVE_ITEM)
        lcd8544_putstr(15, j * LINE_HEIGHT + TOP_SHIFT, menu[LIST_MENU].items[i].text, 0);
      else if (menu[LIST_MENU].items[i].id != SAVE_ITEM)
        lcd8544_putstr(0, j * LINE_HEIGHT + TOP_SHIFT, menu[LIST_MENU].items[i].text, 0);
      
      switch(i) {
      case TEMPERATURE_ITEM:
        lcd8544_putstr(60, j * LINE_HEIGHT + TOP_SHIFT, ftoa(sensor.temperature, 1), 0);
        break;
      case TEMPERATURE_LOW_ITEM:
        lcd8544_putstr(60, j * LINE_HEIGHT + TOP_SHIFT, ftoa(settings.temperatureLow, 1), 0);
        break;
      case TEMPERATURE_HIGHT_ITEM:
        lcd8544_putstr(60, j * LINE_HEIGHT + TOP_SHIFT, ftoa(settings.temperatureHight, 1), 0);
        break;
      case HUMIDITY_ITEM:
       lcd8544_putstr(60, j * LINE_HEIGHT + TOP_SHIFT, ftoa(sensor.humidity, 1), 0);
        break;
      case HUMIDITY_LOW_ITEM:
        lcd8544_putstr(60, j * LINE_HEIGHT + TOP_SHIFT, ftoa(settings.humidityLow, 1), 0);
        break;
      case HUMIDITY_HIGHT_ITEM:
        lcd8544_putstr(60, j * LINE_HEIGHT + TOP_SHIFT, ftoa(settings.humidityHight, 1), 0);
        break;
      case SOUND_DELAY_ITEM:
        lcd8544_dec(soundDelay, 3, 60, j * LINE_HEIGHT + TOP_SHIFT, 0);
        break;
      }
      
  }
  
}

/**
 * Shaping yesno menu
 */ 
void askShapping(void) {

  if (menu[ASK_MENU].position_choice == 0) {
    lcd8544_putstr(15, 1 * LINE_HEIGHT + TOP_SHIFT, menu[ASK_MENU].items[0].text, 1);
    lcd8544_putstr(60, 1 * LINE_HEIGHT + TOP_SHIFT, menu[ASK_MENU].items[1].text, 0);
  } else {
    lcd8544_putstr(15, 1 * LINE_HEIGHT + TOP_SHIFT, menu[ASK_MENU].items[0].text, 0);
    lcd8544_putstr(60, 1 * LINE_HEIGHT + TOP_SHIFT, menu[ASK_MENU].items[1].text, 1);  
  }
  
}

/**
 * Shaping "saved" message
 */ 
void savedShapping(void) {

  lcd8544_putstr(15, 1 * LINE_HEIGHT + TOP_SHIFT, menu[2].items[0].text, 0);
}

unsigned char currentMenu = 0;
bool worked = false;

/**
 * Apply to the display buffer and refresh the display
 */
void show(void) {
  
  if (worked)
    return;
  
  worked = true;
  lcd8544_clear();
  switch(currentMenu) {
  case LIST_MENU:
    batteryShapping();
    listShapping();
    break;
  case ASK_MENU:
    batteryShapping();
    askShapping();
    break;
  case SAVED_MENU:
    batteryShapping();
    savedShapping();
    break;
  }
    
  lcd8544_refresh();
  worked = false;
  
}

/**
 * Handle pressing down button
 */
void moveDown() {
  
  switch(currentMenu) {
  case LIST_MENU:
    int counter = 1;
    while(menu[LIST_MENU].position_choice + counter <= MENU0_ITEMS_MAX - 1) {
      if (menu[LIST_MENU].items[menu[LIST_MENU].position_choice + counter].id != TEMPERATURE_ITEM \
          && menu[LIST_MENU].items[menu[LIST_MENU].position_choice + counter].id != HUMIDITY_ITEM) {
        menu[LIST_MENU].position_choice += counter;
        counter = 0;
        break;
      }
      counter++;
    }
    
    while(menu[LIST_MENU].position_choice - (menu[LIST_MENU].position_view - 1) > ITEMS_TOGETHER) {
      counter = 0;
      menu[LIST_MENU].position_view++;
    }
    
    break;
  case ASK_MENU:
    if (menu[ASK_MENU].position_choice + 1 <= MENU1_ITEMS_MAX - 1)
      menu[ASK_MENU].position_choice++;    
    break;
  case SAVED_MENU:
    
    currentMenu = LIST_MENU;
  
  }
  
  show();
  
}

/**
 * Handle pressing up button
 */
void moveUp() {
  
  switch(currentMenu) {
  case LIST_MENU:
    // increments until temperature or humidity
    int counter = 1;
    while(menu[LIST_MENU].position_choice - counter >= 0) {
      //  if (menu.position_choice - counter < 0)
      //    return;
      if (menu[LIST_MENU].items[menu[LIST_MENU].position_choice - counter].id != TEMPERATURE_ITEM \
          && menu[LIST_MENU].items[menu[LIST_MENU].position_choice - counter].id != HUMIDITY_ITEM) {
        menu[LIST_MENU].position_choice -= counter;
        counter = 0;
        break;
      }
      counter++;
    }

    /*
    было
      if (menu.position_choice - 1 >= 0) {
      //    if (menu.items[menu.position_choice - 1].type != ITEM_TYPE_DROP)
        menu.position_choice--;
      }
      */

    //spin in cycle until displaying
    // hack: first line in menu doesn't dispalying

    while(menu[LIST_MENU].position_choice < menu[LIST_MENU].position_view) {
      counter = 0;
      menu[LIST_MENU].position_view--;
    }

    if (counter > 0) { // if not first element and first droped from selecting
      menu[LIST_MENU].position_view = 0;
    }
    break;
  case ASK_MENU:
    if (menu[ASK_MENU].position_choice - 1 >= 0) {
      menu[ASK_MENU].position_choice--;
    }
    break;
  case SAVED_MENU:
    
    currentMenu = LIST_MENU;
  }
/*
was:
  if (menu.position_choice < menu.position_view)
    menu.position_view--;
*/
  
  show();

}

/**
 * Handle pressing increment button
 */
void valueInc() {
  
  switch(currentMenu) {
  case LIST_MENU:
    if (menu[LIST_MENU].items[menu[LIST_MENU].position_choice].id == TEMPERATURE_LOW_ITEM \
      || menu[LIST_MENU].items[menu[LIST_MENU].position_choice].id == TEMPERATURE_HIGHT_ITEM \
      || menu[LIST_MENU].items[menu[LIST_MENU].position_choice].id == HUMIDITY_LOW_ITEM \
      || menu[LIST_MENU].items[menu[LIST_MENU].position_choice].id == HUMIDITY_HIGHT_ITEM) {
        
        if (*((float*)(menu[LIST_MENU].items[menu[LIST_MENU].position_choice].value)) < 99.9)
          *((float*)(menu[LIST_MENU].items[menu[LIST_MENU].position_choice].value)) += 0.1;
    
    } else if (menu[LIST_MENU].items[menu[LIST_MENU].position_choice].id == SOUND_DELAY_ITEM) { // If sound delay field
      
      if (*((unsigned short*)(menu[LIST_MENU].items[menu[LIST_MENU].position_choice].value)) < 250)
        *((unsigned short*)(menu[LIST_MENU].items[menu[LIST_MENU].position_choice].value)) += 1;
      
      seconds = 0;
      
    } else if (menu[LIST_MENU].items[menu[LIST_MENU].position_choice].id == SAVE_ITEM) { // If YesNo menu
      
      menu[ASK_MENU].position_choice = 0; // No by default
      currentMenu = ASK_MENU;
      
    } 
    break;
  case ASK_MENU:
    if (menu[ASK_MENU].items[menu[ASK_MENU].position_choice].id == NO_ITEM) {
      
      currentMenu = LIST_MENU;
      
    } else if (menu[ASK_MENU].items[menu[ASK_MENU].position_choice].id == YES_ITEM) {
      
      flashErase(MEMORY_PAGE);
      flashWrite((unsigned char *)&settings, MEMORY_PAGE, sizeof(settings));
      currentMenu = SAVED_MENU;
      
    }
    
    break;
  case SAVED_MENU:
    
    currentMenu = LIST_MENU;
  }
  
  show();
  
}

/**
 * Handle pressing decrement button
 */
void valueDec() {
  
  switch(currentMenu) {
  case LIST_MENU:
    if (menu[LIST_MENU].items[menu[LIST_MENU].position_choice].id == TEMPERATURE_LOW_ITEM \
      || menu[LIST_MENU].items[menu[LIST_MENU].position_choice].id == TEMPERATURE_HIGHT_ITEM \
      || menu[LIST_MENU].items[menu[LIST_MENU].position_choice].id == HUMIDITY_LOW_ITEM \
      || menu[LIST_MENU].items[menu[LIST_MENU].position_choice].id == HUMIDITY_HIGHT_ITEM) {
        
        if (*((float*)(menu[LIST_MENU].items[menu[LIST_MENU].position_choice].value)) > 0)
          *((float*)(menu[LIST_MENU].items[menu[LIST_MENU].position_choice].value)) -= 0.1;
    
    } else if (menu[LIST_MENU].items[menu[LIST_MENU].position_choice].id == SOUND_DELAY_ITEM) {
      
      if (*((unsigned short*)(menu[LIST_MENU].items[menu[LIST_MENU].position_choice].value)) > 0)
        *((unsigned short*)(menu[LIST_MENU].items[menu[LIST_MENU].position_choice].value)) -= 1;
      
      seconds = 0;
      
    } else if (menu[LIST_MENU].items[menu[LIST_MENU].position_choice].id == SAVE_ITEM) {
      
      menu[ASK_MENU].position_choice = 0; // No by default
      currentMenu = ASK_MENU;
      
    } 
    break;
  case ASK_MENU:
    
    if (menu[ASK_MENU].items[menu[ASK_MENU].position_choice].id == NO_ITEM) {
      
      currentMenu = LIST_MENU;
      
    } else if (menu[ASK_MENU].items[menu[ASK_MENU].position_choice].id == YES_ITEM) {
      
      flashErase(MEMORY_PAGE);
      flashWrite((unsigned char *)&settings, MEMORY_PAGE, sizeof(settings));
      currentMenu = SAVED_MENU;
      
    }
    
    break;
  case SAVED_MENU:
    
    currentMenu = LIST_MENU;
    
  }
 
  show();
  
}