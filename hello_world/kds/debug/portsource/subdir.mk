################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../portsource/allocate.c \
../portsource/circular_buffer.c \
../portsource/common.c \
../portsource/display.c \
../portsource/dma_driver.c \
../portsource/free.c \
../portsource/help.c \
../portsource/invert.c \
../portsource/main.c \
../portsource/uart.c \
../portsource/uart_driver.c \
../portsource/verify_pattern.c \
../portsource/write.c \
../portsource/writePattern.c 

OBJS += \
./portsource/allocate.o \
./portsource/circular_buffer.o \
./portsource/common.o \
./portsource/display.o \
./portsource/dma_driver.o \
./portsource/free.o \
./portsource/help.o \
./portsource/invert.o \
./portsource/main.o \
./portsource/uart.o \
./portsource/uart_driver.o \
./portsource/verify_pattern.o \
./portsource/write.o \
./portsource/writePattern.o 

C_DEPS += \
./portsource/allocate.d \
./portsource/circular_buffer.d \
./portsource/common.d \
./portsource/display.d \
./portsource/dma_driver.d \
./portsource/free.d \
./portsource/help.d \
./portsource/invert.d \
./portsource/main.d \
./portsource/uart.d \
./portsource/uart_driver.d \
./portsource/verify_pattern.d \
./portsource/write.d \
./portsource/writePattern.d 


# Each subdirectory must supply rules for building sources it contributes
portsource/%.o: ../portsource/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -DDEBUG -DCPU_MKL25Z128VLK4 -DFRDM_KL25Z -DFREEDOM -I../../../../../../platform/osa/inc -I../../../../../../platform/utilities/inc -I../../../../../../platform/CMSIS/Include -I../../../../../../platform/devices -I../../../../../../platform/devices/MKL25Z4/include -I../../../../../../platform/devices/MKL25Z4/startup -I../../../../../../platform/hal/inc -I../../../../../../platform/drivers/inc -I../../../../../../platform/system/inc -I../../../.. -I"/Users/kiyasul/workspace.kds/Freescale/KSDK_1.3.0/examples/frdmkl25z/demo_apps/hello_world/kds/portsource" -I"/Users/kiyasul/workspace.kds/Freescale/KSDK_1.3.0/examples/frdmkl25z/demo_apps/hello_world/kds/portheader" -std=gnu99 -fno-common  -ffreestanding  -fno-builtin  -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


