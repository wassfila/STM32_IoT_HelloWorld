REM $env:Path += ";C:\Users\Wass\.platformio\packages\tool-stlink"
REM SET PATH=%PATH%;C:\Users\Wass\.platformio\packages\tool-stlink
REM st-flash write .\BUILD\BLUEPILL_F103C8\GCC_ARM\08_mbed_cli_os_BluePill_Target.bin 0x08000000
JLink.exe -device STM32F103C8 -If SWD -CommanderScript jlflash.jlink
