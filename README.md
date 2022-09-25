# Microclimate controller (MCC)

[![License](http://img.shields.io/:license-Apache_2.0-blue.svg)](http://badges.mit-license.org)
[![Social](https://img.shields.io/badge/social-telegram-lightgray.svg)](https://teleg.run/c1ewd)
[![IAR](https://img.shields.io/badge/EWARM-8.40.1-orange.svg)](https://www.iar.com)
[![EagleCAD](https://img.shields.io/badge/Eagle-9.5.1-red.svg)](https://www.autodesk.com/products/eagle/overview)
[![Donations](https://img.shields.io/badge/donations-Liberapay-green.svg)](https://liberapay.com/c1ewd/donate)

> Project based STM32F030K6T6

## Dependensies

- CMSIS

## Uses hardware

- TIM1 - sensor
- TIM3_CH3 PWM - backlight
- TIM14 - buttons
- TIM16 - time
- TIM17 - empty

- ADC1_IRQ - battery level

- Internal flash - storage settings

## Clone

> Clone this repo to your local machine using `git clone https://github.com/c1ewd/MCC`

## Features

> Device intended for measuring humidity and temperature. By deviation from specified range of temperature or humidity it send sound signal.
> Hardware design was making in Eagle CAD. Software was coding in IAR.
> Device is using digital humidity and temperature sensor AM2302. For sound signal using EFM-475. For graphical interface using Nokia 5110 display.

## Donations (Optional)

[![Liberapay](https://liberapay.com/assets/widgets/donate.svg)](https://liberapay.com/c1ewd/donate)

## License

- **[Apache 2.0 license](http://www.apache.org/licenses/LICENSE-2.0)**
- Copyright © 2020 <a href="https://teleg.run/c1ewd" target="_blank">c1ewd</a>.
