################################################################################
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include Core/subdir.mk
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
14.LQ_CH32V103_TestALL.hex \

SECONDARY_LIST += \
14.LQ_CH32V103_TestALL.lst \

SECONDARY_SIZE += \
14.LQ_CH32V103_TestALL.siz \


# 所有目标
all: 14.LQ_CH32V103_TestALL.elf secondary-outputs

# 工具调用
14.LQ_CH32V103_TestALL.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -T .ld -nostartfiles -Xlinker --gc-sections -L"../" -Wl,-Map,"14.LQ_CH32V103_TestALL.map" --specs=nano.specs --specs=nosys.specs -o "14.LQ_CH32V103_TestALL.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
14.LQ_CH32V103_TestALL.hex: 14.LQ_CH32V103_TestALL.elf
	@	riscv-none-embed-objcopy -O ihex "14.LQ_CH32V103_TestALL.elf"  "14.LQ_CH32V103_TestALL.hex"
	@	@
14.LQ_CH32V103_TestALL.lst: 14.LQ_CH32V103_TestALL.elf
	@	riscv-none-embed-objdump --source --all-headers --demangle --line-numbers --wide "14.LQ_CH32V103_TestALL.elf" > "14.LQ_CH32V103_TestALL.lst"
	@	@
14.LQ_CH32V103_TestALL.siz: 14.LQ_CH32V103_TestALL.elf
	@	riscv-none-embed-size --format=berkeley "14.LQ_CH32V103_TestALL.elf"
	@	@
# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) 14.LQ_CH32V103_TestALL.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
