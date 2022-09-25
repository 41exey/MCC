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

#ifndef FLASH_H
#define FLASH_H

#include "stm32f030x6.h"
#include "define.h"

/**
 * @struct sSettings
 * Store device settings
 */
#pragma pack(push, 1)
struct sSettings {
  /// variable for fix bug
  float test;
  /// upper limit of humidity
  float humidityHight;
  /// lower limit of humidity 
  float humidityLow;
  /// upper limit of temperature  
  float temperatureHight;
  /// lower limit of temperature
  float temperatureLow;
//  unsigned short soundDelay;
};
#pragma pack(pop)

extern volatile struct sSettings settings, settingsBackup;

void flashErase(unsigned int pageAddress);
void flashWrite(unsigned char* data, unsigned int address, unsigned int count);
void flashRead(unsigned char* data, unsigned int address, unsigned int count);
void flashFirst(void);

#endif
