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
./stm_shared_src/bootloader.o \
./stm_shared_src/flash_memory.o \
./stm_shared_src/secure_boot.o 

C_DEPS += \
./stm_shared_src/bootloader.d \
./stm_shared_src/flash_memory.d \
./stm_shared_src/secure_boot.d 


# Each subdirectory must supply rules for building sources it contributes
stm_shared_src/bootloader.o: D:/CSE/GraduationProject/valeo-GP-FOTA/stm_shared_fota_libs/src/bootloader.c stm_shared_src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F439xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../../../stm_shared_fota_libs/inc -I../wolfSSL -I../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
stm_shared_src/flash_memory.o: D:/CSE/GraduationProject/valeo-GP-FOTA/stm_shared_fota_libs/src/flash_memory.c stm_shared_src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F439xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../../../stm_shared_fota_libs/inc -I../wolfSSL -I../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
stm_shared_src/secure_boot.o: D:/CSE/GraduationProject/valeo-GP-FOTA/stm_shared_fota_libs/src/secure_boot.c stm_shared_src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F439xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../../../stm_shared_fota_libs/inc -I../wolfSSL -I../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-stm_shared_src

clean-stm_shared_src:
	-$(RM) ./stm_shared_src/bootloader.d ./stm_shared_src/bootloader.o ./stm_shared_src/bootloader.su ./stm_shared_src/flash_memory.d ./stm_shared_src/flash_memory.o ./stm_shared_src/flash_memory.su ./stm_shared_src/secure_boot.d ./stm_shared_src/secure_boot.o ./stm_shared_src/secure_boot.su

.PHONY: clean-stm_shared_src

