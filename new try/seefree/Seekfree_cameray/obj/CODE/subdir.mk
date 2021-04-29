################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CODE/cut.c \
../CODE/hcar_init.c \
../CODE/hcar_uart.c \
../CODE/middline.c \
../CODE/ostu.c 

OBJS += \
./CODE/cut.o \
./CODE/hcar_init.o \
./CODE/hcar_uart.o \
./CODE/middline.o \
./CODE/ostu.o 

C_DEPS += \
./CODE/cut.d \
./CODE/hcar_init.d \
./CODE/hcar_uart.d \
./CODE/middline.d \
./CODE/ostu.d 


# Each subdirectory must supply rules for building sources it contributes
CODE/%.o: ../CODE/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"../Libraries/wch_libraries/Core" -I"../Libraries/wch_libraries/Peripheral" -I"../Libraries/wch_libraries/Startup" -I"../Libraries/seekfree_libraries" -I"../Libraries/seekfree_peripheral" -I"../Libraries/board" -I"../CODE" -I"../USER" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

