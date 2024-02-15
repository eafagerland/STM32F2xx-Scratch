TARGET = stm32f207-devboard

BUILD_DIR = build

C_SOURCES = \
	./driver/stm32f2xx_esl_gpio.c \
	./driver/stm32f2xx_esl_timer.c \
	./driver/stm32f2xx_esl_nvic.c \
	./driver/stm32f2xx_esl_rcc.c \
	./driver/stm32f2xx_esl_systick.c \
	./driver/stm32f2xx_esl_uart.c \
	./driver/utilities/eslstring.c \
	./src/main.c \
	./src/gpio.c \
	./src/tim.c \
	./src/nvic.c \
	./src/uart.c
ASM_SOURCES = ./asm/boot.S

PREFIX = arm-none-eabi
CC = $(PREFIX)-gcc
AS = $(PREFIX)-as
CP = $(PREFIX)-objcopy
LD = $(PREFIX)-ld.bfd
SZ = $(PREFIX)-size
GDB = $(PREFIX)-gdb

BIN = $(CP) -O binary

CPU = -mcpu=cortex-m3
MCU = $(CPU) -mthumb -g -std=c99

C_INCLUDES = -Iinc -Idriver -Idriver/utilities
CFLAGS = $(MCU) $(C_INCLUDES)

LDSCRIPT = flash.lds
LDFLAGS = -T $(LDSCRIPT)

all: $(BUILD_DIR)/$(TARGET).bin

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASM_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.S | $(BUILD_DIR)
	$(AS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) | $(BUILD_DIR)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

clean:
	-rm -fR $(BUILD_DIR)

flash:
	st-flash --reset write ./$(BUILD_DIR)/$(TARGET).bin 0x08000000

openocd:
	openocd -f ../utils/openocd.cfg

telnet:
	telnet localhost 4444

gdb: $(BUILD_DIR)/$(TARGET).elf
	$(GDB) --eval-command="target remote localhost:3333" $<