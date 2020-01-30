################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_dma.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_exti.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_gpio.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_rcc.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_usart.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_utils.c 

OBJS += \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_dma.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_exti.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_gpio.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_rcc.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_usart.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_utils.o 

C_DEPS += \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_dma.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_exti.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_gpio.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_rcc.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_usart.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_utils.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32F3xx_HAL_Driver/Src/%.o: ../Drivers/STM32F3xx_HAL_Driver/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DSTM32F303xE -DUSE_FULL_LL_DRIVER '-DHSE_VALUE=8000000' '-DHSE_STARTUP_TIMEOUT=100' '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' '-DEXTERNAL_CLOCK_VALUE=8000000' '-DHSI_VALUE=8000000' '-DLSI_VALUE=40000' '-DVDD_VALUE=3300' '-DPREFETCH_ENABLE=1' -I"D:/Projekty_STM/uart/Inc" -I"D:/Projekty_STM/uart/Drivers/STM32F3xx_HAL_Driver/Inc" -I"D:/Projekty_STM/uart/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"D:/Projekty_STM/uart/Drivers/CMSIS/Include"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


