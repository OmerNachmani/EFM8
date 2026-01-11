################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/efm8ub2/bsp/efm8_memory_lcd/src/spi.c \
../lib/efm8ub2/bsp/efm8_memory_lcd/src/tick.c 

OBJS += \
./lib/efm8ub2/bsp/efm8_memory_lcd/src/spi.OBJ \
./lib/efm8ub2/bsp/efm8_memory_lcd/src/tick.OBJ 


# Each subdirectory must supply rules for building sources it contributes
lib/efm8ub2/bsp/efm8_memory_lcd/src/%.OBJ: ../lib/efm8ub2/bsp/efm8_memory_lcd/src/%.c lib/efm8ub2/bsp/efm8_memory_lcd/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Keil 8051 Compiler'
	wine "/home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/toolchains/keil_8051/9.60/BIN/C51" "@$(patsubst %.OBJ,%.__i,$@)" || $(RC)
	@echo 'Finished building: $<'
	@echo ' '

lib/efm8ub2/bsp/efm8_memory_lcd/src/spi.OBJ: /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/kits/common/bsp/bsp.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/kits/common/drivers/efm8_memory_lcd/inc/spi.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/EFM8UB2/peripheral_driver/inc/spi_0.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/kits/EFM8UB2_SLSTK2001A/config/bsp_config.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/kits/common/drivers/efm8_memory_lcd/inc/config/spi_config.h /home/onachman/SimplicityStudio/v5_workspace/lab7b/inc/config/efm8_config.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/EFM8UB2/inc/SI_EFM8UB2_Register_Enums.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/kits/common/drivers/efm8_memory_lcd/inc/disp.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/EFM8UB2/inc/SI_EFM8UB2_Defs.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/shared/si8051Base/si_toolchain.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/shared/si8051Base/stdint.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/shared/si8051Base/stdbool.h

lib/efm8ub2/bsp/efm8_memory_lcd/src/tick.OBJ: /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/kits/common/bsp/bsp.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/kits/common/drivers/efm8_memory_lcd/inc/tick.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/kits/EFM8UB2_SLSTK2001A/config/bsp_config.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/EFM8UB2/inc/SI_EFM8UB2_Register_Enums.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/EFM8UB2/inc/SI_EFM8UB2_Defs.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/shared/si8051Base/si_toolchain.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/shared/si8051Base/stdint.h /home/onachman/SimplicityStudio5/SimplicityStudio_v5/developer/sdks/8051/v4.3.1/Device/shared/si8051Base/stdbool.h


