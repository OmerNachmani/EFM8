################################################################################
# Automatically-generated file. Do not edit!
################################################################################

export WINEDEBUG=-all

# Keil tools exit code 1 is warnings only, so don't treat as an error in make
RC := test $$? -lt 2

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include lib/efm8ub2/peripheralDrivers/src/subdir.mk
-include lib/efm8ub2/bsp/efm8_memory_lcd/src/subdir.mk
-include lib/bsp/efm8_rgb_led/subdir.mk
-include lib/bsp/efm8_memory_lcd/src/subdir.mk
-include lib/bsp/efm8_joystick/subdir.mk
-include subdir.mk
-include objects.mk

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Lab6_a
BUILD_ARTIFACT_EXTENSION := omf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: Lab6_a.omf

# Tool invocations
Lab6_a.omf: echo_path $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: Keil 8051 Linker'
	wine "/home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/toolchains/keil_8051/9.60/BIN/LX51" "@Lab6_a.lnp" || $(RC)
	@echo 'Finished building target: $@'

# Change from all upper case to the expected case
	@-mv "LAB6_A.OMF.CRBUILD" "$@"

# Generate hex file: Lab6_a.hex
	@wine "/home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/toolchains/keil_8051/9.60/BIN/Ohx51" "Lab6_a.omf" "HEXFILE (Lab6_a.hex)" "H386" 2>&1 >/dev/null

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS) Lab6_a.omf
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
# echo the path
echo_path:
	@echo PATH=$$PATH
	@echo ' '

