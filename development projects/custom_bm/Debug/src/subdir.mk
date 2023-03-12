################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/CSE/GraduationProject/valeo-GP-FOTA/stm_shared_fota_libs/src/bootloader.c \
D:/CSE/GraduationProject/valeo-GP-FOTA/stm_shared_fota_libs/src/flash_memory.c \
D:/CSE/GraduationProject/valeo-GP-FOTA/stm_shared_fota_libs/src/secure_boot.c 

OBJS += \
./src/bootloader.o \
./src/flash_memory.o \
./src/secure_boot.o 

C_DEPS += \
./src/bootloader.d \
./src/flash_memory.d \
./src/secure_boot.d 


# Each subdirectory must supply rules for building sources it contributes
src/bootloader.o: D:/CSE/GraduationProject/valeo-GP-FOTA/stm_shared_fota_libs/src/bootloader.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F439xx -c -I../Core/Inc -I"D:/CSE/GraduationProject/valeo-GP-FOTA/stm_shared_fota_libs/inc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../wolfSSL -I../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/flash_memory.o: D:/CSE/GraduationProject/valeo-GP-FOTA/stm_shared_fota_libs/src/flash_memory.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F439xx -c -I../Core/Inc -I"D:/CSE/GraduationProject/valeo-GP-FOTA/stm_shared_fota_libs/inc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../wolfSSL -I../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/secure_boot.o: D:/CSE/GraduationProject/valeo-GP-FOTA/stm_shared_fota_libs/src/secure_boot.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F439xx -c -I../Core/Inc -I"D:/CSE/GraduationProject/valeo-GP-FOTA/stm_shared_fota_libs/inc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../wolfSSL -I../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src

clean-src:
	-$(RM) ./src/bootloader.d ./src/bootloader.o ./src/bootloader.su ./src/flash_memory.d ./src/flash_memory.o ./src/flash_memory.su ./src/secure_boot.d ./src/secure_boot.o ./src/secure_boot.su

.PHONY: clean-src

