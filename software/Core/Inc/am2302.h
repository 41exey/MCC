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

#ifndef AM2302_H
#define AM2302_H

#include "stm32f030x6.h"
#include "pcd8544.h"
#include "define.h"

bool sensorRead(void);
void TIM1_BRK_UP_TRG_COM_IRQHandler(void);

struct sSensor {
  /// present value of humidity
  float humidity;
  /*
  float humidityHight;
  float humidityLow;
    */
  /// present value of temperature
  float temperature;
  /*
  float temperatureHight;
  float temperatureLow;
    */
  /// stage of polling
  char cmd;
  /// count of sample
  unsigned int count;
  /// bit that is reading
  unsigned char bitCount;
  /// status of reading
  bool status;
  /// sensor data
  uint64_t data;
  /// control summ
  uint8_t crc;
  /// variable for calculation of control summ
  uint8_t crcCalc;
};

extern struct sSensor volatile sensor;

enum {
  SENSOR_RESTART,
  HOST_START_PULL_LOW,
  HOST_START_PULL_UP,
  SENSOR_START_PULL_LOW,
  SENSOR_START_PULL_UP,
  SENSOR_BIT_PULL_LOW,
  SENSOR_BIT_PULL_UP,
  SENSOR_READ_END,
  SENSOR_TIMEOUT,
  SENSOR_READ_COMPLETE,
};

// 100000us
// 100000us / 5us = 20 000 count
#define SENSOR_RESTART_TIME     (unsigned int)20000
// 1 ms
// 1000us / 5us = 200 count
// 18000us / 5us = 3600 count
//#define HOST_START_PULL_LOW_TIME      (uint32_t)200
#define HOST_START_PULL_LOW_TIME      (unsigned int)1000
// 20-40us
// 40us / 5us = 8 count
//#define HOST_START_PULL_UP_TIME       (uint32_t)8
#define HOST_START_PULL_UP_TIME       (unsigned int)16
// 80us
// 80us / 5us = 16 count
//#define SENSOR_START_PULL_LOW_TIMEOUT       (unsigned int)16
#define SENSOR_START_PULL_LOW_TIMEOUT       (unsigned int)100
// -//-
#define SENSOR_START_PULL_UP_TIMEOUT       (unsigned int)100

#define SENSOR_BIT_PULL_LOW_TIMEOUT     (unsigned int)1000

#define SENSOR_BITS_COUNT_MAX (unsigned char)40

// 26 - 28 us
// 40 us / 5 us = 8 count
// 30 us / 5 us = 6 count
#define SENSOR_OUTPUTS_ZERO     (unsigned char)6

// 70us
// 80 us / 5 us = 16 count      
#define SENSOR_OUTPUTE_ONE      (unsigned char)16

#endif