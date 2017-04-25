# Used environements
* [PlatformIO](http://platformio.org/)
* [ARM mbed](https://www.mbed.com/en/)
* [Arduino](https://www.arduino.cc/)

start a new mbed project
```bash
pio init --board bluepill_f103c8 --project-option="framework=mbed"
pio run -v > build.log
pio init --board bluepill_f103c8 --project-option="framework=arduino"
```
```bash
pio run -v | Out-File build.log
```
