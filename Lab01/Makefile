SHELL = cmd.exe
.SHELLFLAGS = /C

MCU = attiny85
F_CPU = 1000000UL
BAUD = 19200
PORT = COM6

PROGRAMMER = stk500v1

main.elf: src/main.c
	avr-gcc -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Wextra -o main.elf src/main.c

main.hex: main.elf
	avr-objcopy -O ihex -R .eeprom main.elf main.hex

flash: main.hex
	avrdude -c $(PROGRAMMER) -p $(MCU) -P$(PORT) -b$(BAUD) -U flash:w:main.hex:i

clean:
	del \Q main.elf main.hex 2>nul