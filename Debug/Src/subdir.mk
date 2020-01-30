################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/frame_parser.c \
../Src/main.c \
../Src/stm32f3xx_it.c \
../Src/syscalls.c \
../Src/system_stm32f3xx.c \
../Src/uart.c 

OBJS += \
./Src/frame_parser.o \
./Src/main.o \
./Src/stm32f3xx_it.o \
./Src/syscalls.o \
./Src/system_stm32f3xx.o \
./Src/uart.o 

C_DEPS += \
./Src/frame_parser.d \
./Src/main.d \
./Src/stm32f3xx_it.d \
./Src/syscalls.d \
./Src/system_stm32f3xx.d \
./Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DSTM32F303xE -DUSE_FULL_LL_DRIVER '-DHSE_VALUE=8000000' '-DHSE_STARTUP_TIMEOUT=100' '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' '-DEXTERNAL_CLOCK_VALUE=8000000' '-DHSI_VALUE=8000000' '-DLSI_VALUE=40000' '-DVDD_VALUE=3300' '-DPREFETCH_ENABLE=1' -I"D:/Projekty_STM/uart/Inc" -I"D:/Projekty_STM/uart/Drivers/STM32F3xx_HAL_Driver/Inc" -I"D:/Projekty_STM/uart/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"D:/Projekty_STM/uart/Drivers/CMSIS/Include"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


