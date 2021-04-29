################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ANO_DT.c \
../User/LQ_ADC.c \
../User/LQ_CAMERA.c \
../User/LQ_DMA.c \
../User/LQ_Encoder.c \
../User/LQ_FLASH.c \
../User/LQ_GPIO.c \
../User/LQ_GPIO_EXIT.c \
../User/LQ_GPIO_KEY.c \
../User/LQ_GPIO_LED.c \
../User/LQ_I2C.c \
../User/LQ_ImageProcess.c \
../User/LQ_Inductor.c \
../User/LQ_MT9V034.c \
../User/LQ_MotorServo.c \
../User/LQ_OLED096.c \
../User/LQ_PID.c \
../User/LQ_PWM.c \
../User/LQ_SPI.c \
../User/LQ_SYS.c \
../User/LQ_TFT18.c \
../User/LQ_TIMER.c \
../User/LQ_UART.c \
../User/Serial_oscilloscope.c \
../User/hcar_encoder.c \
../User/hcar_gogogo.c \
../User/hcar_init.c \
../User/main.c 

OBJS += \
./User/ANO_DT.o \
./User/LQ_ADC.o \
./User/LQ_CAMERA.o \
./User/LQ_DMA.o \
./User/LQ_Encoder.o \
./User/LQ_FLASH.o \
./User/LQ_GPIO.o \
./User/LQ_GPIO_EXIT.o \
./User/LQ_GPIO_KEY.o \
./User/LQ_GPIO_LED.o \
./User/LQ_I2C.o \
./User/LQ_ImageProcess.o \
./User/LQ_Inductor.o \
./User/LQ_MT9V034.o \
./User/LQ_MotorServo.o \
./User/LQ_OLED096.o \
./User/LQ_PID.o \
./User/LQ_PWM.o \
./User/LQ_SPI.o \
./User/LQ_SYS.o \
./User/LQ_TFT18.o \
./User/LQ_TIMER.o \
./User/LQ_UART.o \
./User/Serial_oscilloscope.o \
./User/hcar_encoder.o \
./User/hcar_gogogo.o \
./User/hcar_init.o \
./User/main.o 

C_DEPS += \
./User/ANO_DT.d \
./User/LQ_ADC.d \
./User/LQ_CAMERA.d \
./User/LQ_DMA.d \
./User/LQ_Encoder.d \
./User/LQ_FLASH.d \
./User/LQ_GPIO.d \
./User/LQ_GPIO_EXIT.d \
./User/LQ_GPIO_KEY.d \
./User/LQ_GPIO_LED.d \
./User/LQ_I2C.d \
./User/LQ_ImageProcess.d \
./User/LQ_Inductor.d \
./User/LQ_MT9V034.d \
./User/LQ_MotorServo.d \
./User/LQ_OLED096.d \
./User/LQ_PID.d \
./User/LQ_PWM.d \
./User/LQ_SPI.d \
./User/LQ_SYS.d \
./User/LQ_TFT18.d \
./User/LQ_TIMER.d \
./User/LQ_UART.d \
./User/Serial_oscilloscope.d \
./User/hcar_encoder.d \
./User/hcar_gogogo.d \
./User/hcar_init.d \
./User/main.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"../User" -I"../Startup" -I"../Peripheral/inc" -I"../Peripheral/src" -I"../Debug" -I"../Core" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

