################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CODE/Serial_oscilloscope.c \
../CODE/cut.c \
../CODE/hcar_adc.c \
../CODE/hcar_element.c \
../CODE/hcar_gogogo.c \
../CODE/hcar_init.c \
../CODE/hcar_key.c \
../CODE/hcar_motor.c \
../CODE/hcar_uart.c \
../CODE/hcar_wireless.c \
../CODE/middline.c \
../CODE/ostu.c 

OBJS += \
./CODE/Serial_oscilloscope.o \
./CODE/cut.o \
./CODE/hcar_adc.o \
./CODE/hcar_element.o \
./CODE/hcar_gogogo.o \
./CODE/hcar_init.o \
./CODE/hcar_key.o \
./CODE/hcar_motor.o \
./CODE/hcar_uart.o \
./CODE/hcar_wireless.o \
./CODE/middline.o \
./CODE/ostu.o 

C_DEPS += \
./CODE/Serial_oscilloscope.d \
./CODE/cut.d \
./CODE/hcar_adc.d \
./CODE/hcar_element.d \
./CODE/hcar_gogogo.d \
./CODE/hcar_init.d \
./CODE/hcar_key.d \
./CODE/hcar_motor.d \
./CODE/hcar_uart.d \
./CODE/hcar_wireless.d \
./CODE/middline.d \
./CODE/ostu.d 


# Each subdirectory must supply rules for building sources it contributes
CODE/%.o: ../CODE/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"../Libraries/wch_libraries/Core" -I"../Libraries/wch_libraries/Peripheral" -I"../Libraries/wch_libraries/Startup" -I"../Libraries/seekfree_libraries" -I"../Libraries/seekfree_peripheral" -I"../Libraries/board" -I"../CODE" -I"../USER" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

