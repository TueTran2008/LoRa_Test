################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/LoRa_Drivers/Modules/Message.c 

OBJS += \
./Drivers/LoRa_Drivers/Modules/Message.o 

C_DEPS += \
./Drivers/LoRa_Drivers/Modules/Message.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/LoRa_Drivers/Modules/%.o: ../Drivers/LoRa_Drivers/Modules/%.c Drivers/LoRa_Drivers/Modules/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/Mid/base -I"E:/STM32_Folder/My_Spi_Driver/Drivers/Module/user_uart/cfg" -I"E:/STM32_Folder/My_Spi_Driver/Drivers/Module/user_uart/ip/uart" -I"E:/STM32_Folder/My_Spi_Driver/Drivers/Module/user_uart/ip/usart" -I"E:/STM32_Folder/My_Spi_Driver/Drivers/Module/user_uart/protocol/slip" -I"E:/STM32_Folder/My_Spi_Driver/Drivers/Module/user_uart" -I../Drivers/LoRa_Drivers/Devices/SX127x/hw -I../Drivers/LoRa_Drivers/Devices -I../Core/App -I../Drivers/Module/user_timer -I../Drivers/Module/user_timer/cfg -I"E:/STM32_Folder/My_Spi_Driver/Drivers/Module/user_timer" -I../Drivers/LoRa_Drivers/Devices/SX126x/inc -I../Drivers/LoRa_Drivers/Devices/SX126x/src -I../Drivers/LoRa_Drivers/Devices/SX127x -I../Drivers/LoRa_Drivers/Devices/SX126x/delay -I../Drivers/Module/user_uart -I../Drivers/Module/user_uart/cfg -I../Drivers/Module/user_uart/ip -I../Drivers/Module/user_uart/ip/usart -I../Drivers/Module/user_uart/ip/uart -I../Drivers/Module/user_uart/protocol/slip -I../Drivers/LoRa_Drivers/Modules -I../Drivers/LoRa_Drivers/Modules/PhysicLayer -I../Drivers/LoRa_Drivers/Modules/DataLinkLayer -I../Drivers/LoRa_Drivers/Modules/NetworkLayer -I../Drivers/LoRa_Drivers/Modules/Config -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

