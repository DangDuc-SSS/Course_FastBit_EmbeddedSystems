################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/src/stm32f407_i2c_driver.c \
../drivers/src/stm32f407_spi_driver.c \
../drivers/src/stm32f407_usart_driver.c \
../drivers/src/stm32f407xx_gpio_driver.c \
../drivers/src/stm32f407xx_rcc_driver.c 

OBJS += \
./drivers/src/stm32f407_i2c_driver.o \
./drivers/src/stm32f407_spi_driver.o \
./drivers/src/stm32f407_usart_driver.o \
./drivers/src/stm32f407xx_gpio_driver.o \
./drivers/src/stm32f407xx_rcc_driver.o 

C_DEPS += \
./drivers/src/stm32f407_i2c_driver.d \
./drivers/src/stm32f407_spi_driver.d \
./drivers/src/stm32f407_usart_driver.d \
./drivers/src/stm32f407xx_gpio_driver.d \
./drivers/src/stm32f407xx_rcc_driver.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/src/%.o: ../drivers/src/%.c drivers/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"C:/Users/abdo/Desktop/stm32CubeIDE workspace/stm32f4xx_drivers/bsp" -I"C:/Users/abdo/Desktop/stm32CubeIDE workspace/stm32f4xx_drivers/drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

