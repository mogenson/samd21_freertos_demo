# Project name. Determines the name of the built binaries.
PRJ_NAME = samd21_freertos_demo

# Path to top level ASF directory relative to this project directory.
PRJ_PATH = $(CURDIR)

# ASF source code library directory name
ASF = xdk-asf-3.34.2

# Target CPU architecture: cortex-m3, cortex-m4
ARCH = cortex-m0plus

# Target part: none, sam3n4 or sam4l4aa
PART = samd21j18a

# Create a directory to compile all sources and place generated binaries in
BUILD_DIR = build/

# Application target name.
# Given with suffix .a for library and .elf for a standalone application.
TARGET_FLASH = $(BUILD_DIR)$(PRJ_NAME)_flash.elf
TARGET_SRAM = $(BUILD_DIR)$(PRJ_NAME)_sram.elf

# List of C source files.
CSRCS = \
	$(ASF)/common/services/usb/class/cdc/device/udi_cdc.c \
	$(ASF)/common/services/usb/class/cdc/device/udi_cdc_desc.c \
	$(ASF)/common/services/usb/udc/udc.c \
	$(ASF)/common/utils/interrupt/interrupt_sam_nvic.c \
	$(ASF)/sam0/boards/samd21_xplained_pro/board_init.c \
	$(ASF)/sam0/drivers/extint/extint_callback.c \
	$(ASF)/sam0/drivers/extint/extint_sam_d_r_h/extint.c \
	$(ASF)/sam0/drivers/port/port.c \
	$(ASF)/sam0/drivers/rtc/rtc_sam_d_r_h/rtc_count.c \
	$(ASF)/sam0/drivers/system/clock/clock_samd21_r21_da_ha1/clock.c \
	$(ASF)/sam0/drivers/system/clock/clock_samd21_r21_da_ha1/gclk.c \
	$(ASF)/sam0/drivers/system/interrupt/system_interrupt.c \
	$(ASF)/sam0/drivers/system/pinmux/pinmux.c \
	$(ASF)/sam0/drivers/system/system.c \
	$(ASF)/sam0/drivers/usb/stack_interface/usb_device_udd.c  \
	$(ASF)/sam0/drivers/usb/stack_interface/usb_dual.c \
	$(ASF)/sam0/drivers/usb/usb_sam_d_r/usb.c \
	$(ASF)/sam0/utils/cmsis/samd21/source/gcc/startup_samd21.c \
	$(ASF)/sam0/utils/cmsis/samd21/source/system_samd21.c \
	$(ASF)/sam0/utils/syscalls/gcc/syscalls.c \
	freertos/Source/FreeRTOS_CLI.c \
	freertos/Source/croutine.c \
	freertos/Source/event_groups.c \
	freertos/Source/list.c \
	freertos/Source/portable/GCC/ARM_CM0/port.c \
	freertos/Source/portable/MemMang/heap_4.c \
	freertos/Source/queue.c \
	freertos/Source/tasks.c \
	freertos/Source/timers.c \
	src/cli_task.c \
	src/led_task.c \
	src/main.c \
	src/printf_stdarg.c

# List of assembler source files.
ASSRCS =

# List of include paths.
INC_PATH = \
	$(ASF)/common/boards \
	$(ASF)/common/services/sleepmgr \
	$(ASF)/common/services/usb \
	$(ASF)/common/services/usb/class/cdc \
	$(ASF)/common/services/usb/class/cdc/device \
	$(ASF)/common/services/usb/udc \
	$(ASF)/common/utils \
	$(ASF)/sam0/boards \
	$(ASF)/sam0/boards/samd21_xplained_pro \
	$(ASF)/sam0/drivers/extint \
	$(ASF)/sam0/drivers/extint/extint_sam_d_r_h \
	$(ASF)/sam0/drivers/port \
	$(ASF)/sam0/drivers/rtc \
	$(ASF)/sam0/drivers/rtc/rtc_sam_d_r_h \
	$(ASF)/sam0/drivers/system \
	$(ASF)/sam0/drivers/system/clock \
	$(ASF)/sam0/drivers/system/clock/clock_samd21_r21_da_ha1 \
	$(ASF)/sam0/drivers/system/interrupt \
	$(ASF)/sam0/drivers/system/interrupt/system_interrupt_samd21 \
	$(ASF)/sam0/drivers/system/pinmux \
	$(ASF)/sam0/drivers/system/power \
	$(ASF)/sam0/drivers/system/power/power_sam_d_r_h \
	$(ASF)/sam0/drivers/system/reset \
	$(ASF)/sam0/drivers/system/reset/reset_sam_d_r_h \
	$(ASF)/sam0/drivers/usb \
	$(ASF)/sam0/drivers/usb/stack_interface \
	$(ASF)/sam0/drivers/usb/usb_sam_d_r \
	$(ASF)/sam0/utils \
	$(ASF)/sam0/utils/cmsis/samd21/include \
	$(ASF)/sam0/utils/cmsis/samd21/source \
	$(ASF)/sam0/utils/header_files \
	$(ASF)/sam0/utils/preprocessor \
	$(ASF)/thirdparty/CMSIS/Include \
	$(ASF)/thirdparty/CMSIS/Lib/GCC \
	freertos/Source/include \
	freertos/Source/portable/GCC/ARM_CM0 \
	src \
	src/config

# Additional search paths for libraries.
LIB_PATH =  \
	$(ASF)/thirdparty/CMSIS/Lib/GCC

# List of libraries to use during linking.
LIBS =  \
	arm_cortexM0l_math

# Path relative to top level directory pointing to a linker script.
LINKER_SCRIPT_FLASH = $(ASF)/sam0/utils/linker_scripts/samd21/gcc/samd21j18a_flash.ld
LINKER_SCRIPT_SRAM  = $(ASF)/sam0/utils/linker_scripts/samd21/gcc/samd21j18a_sram.ld

# Path relative to top level directory pointing to a linker script.
DEBUG_SCRIPT_FLASH = $(ASF)/sam0/boards/samd21_xplained_pro/debug_scripts/gcc/samd21_xplained_pro_flash.gdb
DEBUG_SCRIPT_SRAM  = $(ASF)/sam0/boards/samd21_xplained_pro/debug_scripts/gcc/samd21_xplained_pro_sram.gdb

# Project type parameter: all, sram or flash
PROJECT_TYPE = flash

# Additional options for debugging. By default the common Makefile.in will
# add -g3.
DBGFLAGS =

# Application optimization used during compilation and linking:
# -O0, -O1, -O2, -O3 or -Os
OPTIMIZATION = -O1

# Extra flags to use when archiving.
ARFLAGS =

# Extra flags to use when assembling.
ASFLAGS =

# Extra flags to use when compiling.
CFLAGS =

# Extra flags to use when preprocessing.
#
# Preprocessor symbol definitions
#   To add a definition use the format "-D name[=definition]".
#   To cancel a definition use the format "-U name".
#
# The most relevant symbols to define for the preprocessor are:
#   BOARD      Target board in use, see boards/board.h for a list.
#   EXT_BOARD  Optional extension board in use, see boards/board.h for a list.
CPPFLAGS = \
	-D ARM_MATH_CM0PLUS=true                           \
	-D BOARD=SAMD21_XPLAINED_PRO                       \
	-D EXTINT_CALLBACK_MODE=true                       \
	-D RTC_COUNT_ASYNC=false                           \
	-D UDD_ENABLE                                      \
	-D __FREERTOS__                                    \
	-D __SAMD21J18A__

# Extra flags to use when linking
LDFLAGS =

# Pre- and post-build commands
PREBUILD_CMD =
POSTBUILD_CMD =
