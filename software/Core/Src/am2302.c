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

#include "am2302.h"
//#include "display.h"

struct sSensor volatile sensor = {0};

/**
 * Sensor poll wrapper
 */
bool sensorRead(void) {

  sensor.status = false;
  sensor.count = 0;
  sensor.bitCount = 0;
  sensor.data = 0;  
  sensor.crc = 0;
  
  sensor.cmd = SENSOR_RESTART;

  TIM1->CR1 |= TIM_CR1_CEN;
  while(!sensor.status);
  
  if (sensor.cmd == SENSOR_TIMEOUT) {
    /*
    lcd8544_clear();
    lcd8544_putstr(0, 15, "ÒÀÉÌÀÓÒ", 0);
    lcd8544_refresh();
*/
    return false;
  } else if (sensor.cmd == SENSOR_READ_COMPLETE) {
    /*
    lcd8544_clear();
    lcd8544_dec(sensor.humidity, 4, 3, 3, 0);
    lcd8544_dec(sensor.temperature, 4, 3, 10, 0);
    lcd8544_dec(sensor.crc, 4, 3, 17, 0);
    lcd8544_dec(sensor.crcCalc, 4, 3, 24, 0);
    lcd8544_refresh();
*/
//    menuView();
    return true;
  } else {
    /*
    lcd8544_clear();
    lcd8544_putstr(0, 15, "ÍÈ×ÅÃÎ", 0);
    lcd8544_refresh();
*/
  }

  return false;
}

/**
 * Main poll of the sensor
 */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{

  switch(sensor.cmd) {
  case SENSOR_RESTART:
    if (sensor.count == 0) {
      GPIOB->ODR &= ~GPIO_ODR_3;
      GPIOB->ODR |= GPIO_ODR_3;
    }
    
    sensor.count++;
    
    if (sensor.count >= SENSOR_RESTART_TIME) {
      sensor.cmd = HOST_START_PULL_LOW;
      sensor.count = 0;
    }
    
    break;
  case HOST_START_PULL_LOW:
    
    if (sensor.count == 0)
      GPIOB->ODR &= ~GPIO_ODR_3;
    
    sensor.count++;
    
    if (sensor.count >= HOST_START_PULL_LOW_TIME) {
      sensor.cmd = HOST_START_PULL_UP;
      sensor.count = 0;
    }
    
    break;
  case HOST_START_PULL_UP:
    
    if (sensor.count == 0)
      GPIOB->ODR |= GPIO_ODR_3;
    
    sensor.count++;
    
    if ((GPIOB->IDR & GPIO_IDR_3) == 0) {
      sensor.cmd = SENSOR_START_PULL_LOW;
      sensor.count = 0;
    }
    
    if (sensor.count >= SENSOR_START_PULL_LOW_TIMEOUT) {
      sensor.cmd = SENSOR_TIMEOUT;
    }
    
    break;
  case SENSOR_START_PULL_LOW:
    if (GPIOB->IDR & GPIO_IDR_3) { 
      sensor.cmd = SENSOR_START_PULL_UP;
      sensor.count = 0;
    }    
    
    if (sensor.count >= SENSOR_START_PULL_UP_TIMEOUT) {
      sensor.cmd = SENSOR_TIMEOUT;
    }

    sensor.count++;
    break;
  case SENSOR_START_PULL_UP: // dont working
    
    if ((GPIOB->IDR & GPIO_IDR_3) == 0) {
      sensor.cmd = SENSOR_BIT_PULL_LOW;
      sensor.bitCount = SENSOR_BITS_COUNT_MAX;
      sensor.count = 0;
    }

    if (sensor.count >= 100000) {
      sensor.cmd = SENSOR_TIMEOUT;
    }
    
    sensor.count++;
    break;
  case SENSOR_BIT_PULL_LOW:
    
    sensor.count++;
    
    if (GPIOB->IDR & GPIO_IDR_3) {
      sensor.cmd = SENSOR_BIT_PULL_UP;
      sensor.count = 0;
    }
    
    if (sensor.bitCount == 0) {
      sensor.cmd = SENSOR_READ_END;
      sensor.count = 0;      
    }
    
    break;
  case SENSOR_BIT_PULL_UP:
    
    sensor.count++;
    
    if ((GPIOB->IDR & GPIO_IDR_3) == 0) {
      sensor.cmd = SENSOR_BIT_PULL_LOW;
      sensor.bitCount--;

      if (sensor.count > SENSOR_OUTPUTS_ZERO) {
        sensor.data |= (1ULL << sensor.bitCount);
     } else {
        sensor.data &= ~(1ULL << sensor.bitCount);
     }

      sensor.count = 0;
    }    
    break;
  case SENSOR_TIMEOUT:
    TIM1->CR1 &= ~TIM_CR1_CEN;
    sensor.status = true;
    break;
  case SENSOR_READ_END:
    sensor.crcCalc = (uint8_t)(((sensor.data & 0xff000000ULL)>>24) \
                                + ((sensor.data & 0xff00000000ULL)>>32) \
                                + ((sensor.data & 0x0000ff00ULL)>>8) \
                                + ((sensor.data & 0x00ff0000ULL)>>16));
    
    sensor.humidity = (uint16_t)(sensor.data >> 24)/10.;
    sensor.temperature = (uint16_t)(sensor.data >> 8)/10.;   
    sensor.crc = (uint8_t)sensor.data;
    sensor.cmd = SENSOR_READ_COMPLETE;

    if (sensor.crc == sensor.crcCalc) 
    {    
       // CRC OK
    }
    
    break;
  case SENSOR_READ_COMPLETE:
    TIM1->CR1 &= ~TIM_CR1_CEN;
    sensor.status = true;
    sensor.cmd = SENSOR_READ_COMPLETE;
    break;
  }
  
  TIM1->SR &=~ TIM_SR_UIF;
}
