################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Avanzamento.c \
../Core/Src/Impostazioni.c \
../Core/Src/PosizioneMandrino.c \
../Core/Src/VelocitaMandrino.c \
../Core/Src/VirtualEeprom.c \
../Core/Src/ee.c \
../Core/Src/i2c-lcd.c \
../Core/Src/main.c \
../Core/Src/menuPrincipale.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c 

OBJS += \
./Core/Src/Avanzamento.o \
./Core/Src/Impostazioni.o \
./Core/Src/PosizioneMandrino.o \
./Core/Src/VelocitaMandrino.o \
./Core/Src/VirtualEeprom.o \
./Core/Src/ee.o \
./Core/Src/i2c-lcd.o \
./Core/Src/main.o \
./Core/Src/menuPrincipale.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o 

C_DEPS += \
./Core/Src/Avanzamento.d \
./Core/Src/Impostazioni.d \
./Core/Src/PosizioneMandrino.d \
./Core/Src/VelocitaMandrino.d \
./Core/Src/VirtualEeprom.d \
./Core/Src/ee.d \
./Core/Src/i2c-lcd.d \
./Core/Src/main.d \
./Core/Src/menuPrincipale.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/Avanzamento.d ./Core/Src/Avanzamento.o ./Core/Src/Impostazioni.d ./Core/Src/Impostazioni.o ./Core/Src/PosizioneMandrino.d ./Core/Src/PosizioneMandrino.o ./Core/Src/VelocitaMandrino.d ./Core/Src/VelocitaMandrino.o ./Core/Src/VirtualEeprom.d ./Core/Src/VirtualEeprom.o ./Core/Src/ee.d ./Core/Src/ee.o ./Core/Src/i2c-lcd.d ./Core/Src/i2c-lcd.o ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/menuPrincipale.d ./Core/Src/menuPrincipale.o ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o

.PHONY: clean-Core-2f-Src
