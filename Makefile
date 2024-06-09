TARGET = stm32f207-devboard

BUILD_DIR = build

C_SOURCES = \
	./rtos/src/os_kernel.c \
	./rtos/src/os_semaphore.c \
	./driver/stm32f2xx/stm32f2xx_esl.c \
	./driver/stm32f2xx/stm32f2xx_esl_gpio.c \
	./driver/stm32f2xx/stm32f2xx_esl_timer.c \
	./driver/stm32f2xx/stm32f2xx_esl_nvic.c \
	./driver/stm32f2xx/stm32f2xx_esl_rcc.c \
	./driver/stm32f2xx/stm32f2xx_esl_systick.c \
	./driver/stm32f2xx/stm32f2xx_esl_uart.c \
	./driver/stm32f2xx/stm32f2xx_esl_pwr.c \
	./driver/stm32f2xx/stm32f2xx_esl_rtc.c \
	./driver/stm32f2xx/stm32f2xx_esl_i2c.c \
	./driver/utilities/eslstring.c \
	./core/src/main.c \
	./core/src/gpio.c \
	./core/src/tim.c \
	./core/src/nvic.c \
	./core/src/uart.c \
	./core/src/interrupts.c \
	./core/src/rtc.c \
	./core/src/i2c.c
ASM_SOURCES = ./asm/boot.S

PREFIX = arm-none-eabi
CC = $(PREFIX)-gcc
AS = $(PREFIX)-as
CP = $(PREFIX)-objcopy
LD = $(PREFIX)-ld
SZ = $(PREFIX)-size
GDB = $(PREFIX)-gdb

BIN = $(CP) -O binary

CPU = -mcpu=cortex-m3
MCU = $(CPU) -mthumb -g -std=c99

C_INCLUDES = -Icore/inc -Idriver -Idriver/utilities -Idriver/stm32f2xx -Irtos/inc
CFLAGS = $(MCU) $(C_INCLUDES)
CFLAGS += -Wall

LDSCRIPT = flash.lds
LDFLAGS = -T $(LDSCRIPT)

LIBS = -L"C:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\13.2 Rel1\lib\gcc\arm-none-eabi\13.2.1\thumb\v7-m\nofp" -lm
LIBS += -L"C:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\13.2 Rel1\lib\gcc\arm-none-eabi\13.2.1\thumb\v7-m\nofp" -lgcc

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
	$(LD) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@
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