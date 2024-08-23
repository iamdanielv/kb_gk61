# GK61 Custom iamdanielv

![GK61 PRO DS](https://i.imgur.com/x7riwSeh.png)

## The PCB features:
* QMK & VIA compatibility
* RGB Matrix backlight

The following is the QMK Firmware for the Destop 60% keylayout -  designed in Dongguan Jizhi Electronic Technology Co., Ltd

* Keyboard Maintainer: [iamdanielv](https://github.com/iamdanielv)
* Hardware Supported: DestopPCB for Skyloong keylayout 60%, STM32F103C8T6
* Hardware Availability: http://www.skyloong.com.cn  https://skyloong.vip

Make example for this keyboard (after setting up your build environment):

    make skyloong/gk61/iamdanielv:default

Flashing example for this keyboard:

    make skyloong/gk61/iamdanielv:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix (usually the top left key or Escape) and plug in the keyboard
* **Physical reset button**: Press the physical button on the PCB, under the space bar on right side
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
