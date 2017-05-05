# Used environements
* [PlatformIO](http://platformio.org/)
* [ARM mbed](https://www.mbed.com/en/)
* [Arduino](https://www.arduino.cc/)

# How to use these samples with mbed cli
First clone this repo
```bash
cd 08_mbed_cli_os_BluePill_Target
mbed deploy
mbed compile
```
you might notice that the example 08 fills the flash almost completely, that's why in project 09 I used the nano.specs flags in json config file, to make the compilation easier I added batch scripts
```bash
cd 09_mbed_cli_nRF
mbed deploy
compile.bat
flash.bat
```
After an edit cycle to directly compile and run
```bash
deploy.bat
```
# Samples description
* 03_pio_mbed_Pwm : pwm signal on pin PB_4 with 20 ms period
* 04_pio_mbed_uart : 2 UARTS Serial3 for log and Serial2 with uart callback echo character
* 05_pio_mbed_nRF : Empty Project where I faced troubles including mbed libraries from pio, so switched to mbed cli for a better integration with the mbed libraries management, also after the lib_nano fitting in the blue pill, mbed is taken as the focus solution
* 06_mbed_cli_Hello_BluePill : mbed 2 simple examples
* 07_mbed_cli_os_Hello_BluePill : mbed 5 aka mbed-os simple example
* 08_mbed_cli_os_BluePill_Target : from this onwards I patched the mbed-os to support the Blue pill directly so no need anymore to use a different board Nucleo with a different device.
* 09_mbed_cli_nRF : nRF24L01p is configured with receiving parameters of the Home Smart Mesh Network and displayout hex data on the serial port

# mBed online Samples
* [mbed_usb_serial](https://developer.mbed.org/users/hudakz/code/STM32F103C8T6_USBSerial/)
  
  Tested on the STM32 Blue pill, working fine, some troubles with usb through resets.

# More Info
## start a new mbed project with PlatformIO
```bash
pio init --board bluepill_f103c8 --project-option="framework=mbed"
pio run -v > build.log
pio init --board bluepill_f103c8 --project-option="framework=arduino"
platformio run --target upload -v
```
```bash
pio run -v | Out-File build.log
```
