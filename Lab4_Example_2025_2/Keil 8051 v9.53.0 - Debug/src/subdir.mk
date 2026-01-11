################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
A51_UPPER_SRCS += \
/home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/shared/si8051Base/SILABS_STARTUP.A51 

C_SRCS += \
../src/InitDevice.c \
../src/main.c 

OBJS += \
./src/InitDevice.OBJ \
./src/SILABS_STARTUP.OBJ \
./src/main.OBJ 


# Each subdirectory must supply rules for building sources it contributes
src/%.OBJ: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Keil 8051 Compiler'
	wine "/home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/toolchains/keil_8051/9.60/BIN/C51" "@$(patsubst %.OBJ,%.__i,$@)" || $(RC)
	@echo 'Finished building: $<'
	@echo ' '

src/InitDevice.OBJ: /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/EFM8UB2/inc/SI_EFM8UB2_Register_Enums.h /home/onachman/SimplicityStudio/v5_workspace/Lab4_Example_2025_2/inc/InitDevice.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/EFM8UB2/inc/SI_EFM8UB2_Defs.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/shared/si8051Base/si_toolchain.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/shared/si8051Base/stdint.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/shared/si8051Base/stdbool.h

src/SILABS_STARTUP.OBJ: /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/shared/si8051Base/SILABS_STARTUP.A51 src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Keil 8051 Assembler'
	wine "/home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/toolchains/keil_8051/9.60/BIN/AX51" "@$(patsubst %.OBJ,%.__ia,$@)" || $(RC)
	@echo 'Finished building: $<'
	@echo ' '

src/main.OBJ: /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/kits/common/bsp/bsp.h /home/onachman/SimplicityStudio/v5_workspace/Lab4_Example_2025_2/inc/InitDevice.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/kits/common/drivers/efm8_memory_lcd/inc/disp.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/kits/common/drivers/efm8_memory_lcd/inc/tick.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/kits/common/drivers/efm8_memory_lcd/inc/render.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/kits/EFM8UB2_SLSTK2001A/config/bsp_config.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/kits/common/drivers/efm8_memory_lcd/inc/config/render_config.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/EFM8UB2/inc/SI_EFM8UB2_Register_Enums.h /home/onachman/SimplicityStudio/v5_workspace/Lab4_Example_2025_2/inc/config/memory_lcd_config.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/EFM8UB2/inc/SI_EFM8UB2_Defs.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/shared/si8051Base/si_toolchain.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/shared/si8051Base/stdint.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/shared/si8051Base/stdbool.h


