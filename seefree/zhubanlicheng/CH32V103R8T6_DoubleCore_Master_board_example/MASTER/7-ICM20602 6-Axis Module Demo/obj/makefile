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
7-ICM20602\ 6-Axis\ Module\ Demo.hex \

SECONDARY_LIST += \
7-ICM20602\ 6-Axis\ Module\ Demo.lst \

SECONDARY_SIZE += \
7-ICM20602\ 6-Axis\ Module\ Demo.siz \


# 所有目标
all: 7-ICM20602\ 6-Axis\ Module\ Demo.elf secondary-outputs

# 工具调用
7-ICM20602\ 6-Axis\ Module\ Demo.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -T .ld -nostartfiles -Xlinker --gc-sections -L"../" -Wl,-Map,"7-ICM20602 6-Axis Module Demo.map" --specs=nano.specs --specs=nosys.specs -o "7-ICM20602 6-Axis Module Demo.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
7-ICM20602\ 6-Axis\ Module\ Demo.hex: 7-ICM20602\ 6-Axis\ Module\ Demo.elf
	@	riscv-none-embed-objcopy -O ihex "7-ICM20602 6-Axis Module Demo.elf"  "7-ICM20602 6-Axis Module Demo.hex"
	@	@
7-ICM20602\ 6-Axis\ Module\ Demo.lst: 7-ICM20602\ 6-Axis\ Module\ Demo.elf
	@	riscv-none-embed-objdump --source --all-headers --demangle --line-numbers --wide "7-ICM20602 6-Axis Module Demo.elf" > "7-ICM20602 6-Axis Module Demo.lst"
	@	@
7-ICM20602\ 6-Axis\ Module\ Demo.siz: 7-ICM20602\ 6-Axis\ Module\ Demo.elf
	@	riscv-none-embed-size --format=berkeley "7-ICM20602 6-Axis Module Demo.elf"
	@	@
# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) "7-ICM20602 6-Axis Module Demo.elf"
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
