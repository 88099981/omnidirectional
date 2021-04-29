################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Startup/ch32v10x_it.c \
../Startup/system_ch32v10x.c 

S_UPPER_SRCS += \
../Startup/startup_ch32v10x.S 

OBJS += \
./Startup/ch32v10x_it.o \
./Startup/startup_ch32v10x.o \
./Startup/system_ch32v10x.o 

S_UPPER_DEPS += \
./Startup/startup_ch32v10x.d 

C_DEPS += \
./Startup/ch32v10x_it.d \
./Startup/system_ch32v10x.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/%.o: ../Startup/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"../User" -I"../Startup" -I"../Peripheral/inc" -I"../Peripheral/src" -I"../Debug" -I"../Core" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
Startup/%.o: ../Startup/%.S
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -x assembler -I"../Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

