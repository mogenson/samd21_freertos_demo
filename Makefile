# List of available make goals:
#
# all                     Default target, builds the project
# clean                   Clean up the project
# rebuild                 Rebuild the project
# flash                   Flash binary to the samd21 xplained dev board
# erase                   Erase the samd21 xplained dev board
#
#
# doc                     Build the documentation
# cleandoc                Clean up the documentation
# rebuilddoc              Rebuild the documentation

# Source the common SAM0 Makefile from the ASF directory. The common Makefile
# contains the make targets (rebuild, clean, etc). The common Makefile also
# sources the project specifc settings (like C sources and build directory)
# from the config.mk file inlcuded in the same direcotry as this Makefile.

MAKEFILE_PATH = xdk-asf-3.34.2/sam0/utils/make/Makefile.sam.in
include $(MAKEFILE_PATH)

ifeq ($(os_type),Darwin)   # check if Mac OS X
# redefine RMDIR, the original rmdir command will fail for non-empty directories
RMDIR := rm -rf
endif

# use the edbg utility to flash a binary to the samd21 xplained board via the
# CMSIS-DAP debug IC.
.PHONY: flash
flash: all
	@echo
	@echo "Flashing $(project).bin to samd21 xplained dev board"
	@echo
	$(Q)edbg/edbg --verbose --program --verify --target atmel_cm0p --file $(project).bin

.PHONY: erase
erase:
	@echo
	@echo "Erasing samd21 xplained dev board"
	@echo
	$(Q)edbg/edbg --verbose --erase --target atmel_cm0p
