################################################################################
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include USER/subdir.mk
-include Libraries/wch_libraries/Startup/subdir.mk
-include Libraries/wch_libraries/Peripheral/subdir.mk
-include Libraries/wch_libraries/Core/subdir.mk
-include Libraries/seekfree_peripheral/subdir.mk
-include Libraries/seekfree_libraries/subdir.mk
-include Libraries/board/subdir.mk
-include CODE/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
Seekfree_motor.hex \

SECONDARY_LIST += \
Seekfree_motor.lst \

SECONDARY_SIZE += \
Seekfree_motor.siz \


# 所有目标
all: Seekfree_motor.elf secondary-outputs

# 工具调用
Seekfree_motor.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -T .ld -nostartfiles -Xlinker --gc-sections -L"../" -Wl,-Map,"Seekfree_motor.map" --specs=nano.specs --specs=nosys.specs -o "Seekfree_motor.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
Seekfree_motor.hex: Seekfree_motor.elf
	@	riscv-none-embed-objcopy -O ihex "Seekfree_motor.elf"  "Seekfree_motor.hex"
	@	@
Seekfree_motor.lst: Seekfree_motor.elf
	@	riscv-none-embed-objdump --source --all-headers --demangle --line-numbers --wide "Seekfree_motor.elf" > "Seekfree_motor.lst"
	@	@
Seekfree_motor.siz: Seekfree_motor.elf
	@	riscv-none-embed-size --format=berkeley "Seekfree_motor.elf"
	@	@
# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) Seekfree_motor.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
