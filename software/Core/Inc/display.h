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

#ifndef MENU_H
#define MENU_H

#include "pcd8544.h"
#include "am2302.h"
#include "battery.h"
#include "define.h"

#define MENU0_ITEMS_MAX        8
#define MENU1_ITEMS_MAX        2
#define ITEMS_TOGETHER   4
#define TOP_SHIFT       10

#define LINE_HEIGHT     10

#define LENGTH  13

/*
enum ITEM_TYPE {
  ITEM_TYPE_REGULAR,
  ITEM_TYPE_DROP,
  ITEM_TYPE_ENUM,
  ITEM_TYPE_MENU,
  M_ITEM_TYPE
};
*/

/**
 * @struct sItem
 * Item of menu struct
 */ 
struct sItem {
  /// text of item
  const unsigned char text[LENGTH];
  /// identifier
  const unsigned char id;
  /// pointer to sensor or setting value
  const void *value;
};

/**
 * @struct sMenu
 * Displaying menu
 */ 
struct sMenu {
  /// present position
  unsigned char position_choice;
  /// array of items
  struct sItem const items[MENU0_ITEMS_MAX];
  /// visible window
  unsigned char position_view;
};

enum MENU {
  LIST_MENU,
  ASK_MENU,
  SAVED_MENU,
  M_MENU
};

enum ITEMS {
  TEMPERATURE_ITEM,
  TEMPERATURE_LOW_ITEM,
  TEMPERATURE_HIGHT_ITEM,
  HUMIDITY_ITEM,
  HUMIDITY_LOW_ITEM,
  HUMIDITY_HIGHT_ITEM,
  SOUND_DELAY_ITEM,
  SAVE_ITEM,
  NO_ITEM,
  YES_ITEM,
  SAVED_ITEM,
  M_ITEM
};

//extern unsigned char volatile soundDelay;

extern struct sMenu menu[3];

void listShapping(void);
void askShapping(void);
void savedShapping(void);
void show(void);
void moveDown(void);
void moveUp(void);
void valueInc(void);
void valueDec(void);

#endif