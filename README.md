# Used environements
* [PlatformIO](http://platformio.org/)
* [ARM mbed](https://www.mbed.com/en/)
* [Arduino](https://www.arduino.cc/)

start a new mbed project
```bash
pio init --board bluepill_f103c8 --project-option="framework=mbed"
pio run -v > build.log
pio init --board bluepill_f103c8 --project-option="framework=arduino"
platformio run --target upload -v
```
```bash
pio run -v | Out-File build.log
```
# Samples description
* 03_pio_mbed_Pwm : pwm signal on pin PB_4 with 20 ms period
* 04_pio_mbed_uart : 2 UARTS Serial3 for log and Serial2 with uart callback echo character

# mBed online Samples
* [mbed_usb_serial](https://developer.mbed.org/users/hudakz/code/STM32F103C8T6_USBSerial/)
  
  Tested on the STM32 Blue pill, working fine, some troubles with usb through resets.
