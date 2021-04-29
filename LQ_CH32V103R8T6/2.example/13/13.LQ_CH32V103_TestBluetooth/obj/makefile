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
13.LQ_CH32V103_TestBluetooth.hex \

SECONDARY_LIST += \
13.LQ_CH32V103_TestBluetooth.lst \

SECONDARY_SIZE += \
13.LQ_CH32V103_TestBluetooth.siz \


# 所有目标
all: 13.LQ_CH32V103_TestBluetooth.elf secondary-outputs

# 工具调用
13.LQ_CH32V103_TestBluetooth.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -T .ld -nostartfiles -Xlinker --gc-sections -L"../" -Wl,-Map,"13.LQ_CH32V103_TestBluetooth.map" --specs=nano.specs --specs=nosys.specs -o "13.LQ_CH32V103_TestBluetooth.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
13.LQ_CH32V103_TestBluetooth.hex: 13.LQ_CH32V103_TestBluetooth.elf
	@	riscv-none-embed-objcopy -O ihex "13.LQ_CH32V103_TestBluetooth.elf"  "13.LQ_CH32V103_TestBluetooth.hex"
	@	@
13.LQ_CH32V103_TestBluetooth.lst: 13.LQ_CH32V103_TestBluetooth.elf
	@	riscv-none-embed-objdump --source --all-headers --demangle --line-numbers --wide "13.LQ_CH32V103_TestBluetooth.elf" > "13.LQ_CH32V103_TestBluetooth.lst"
	@	@
13.LQ_CH32V103_TestBluetooth.siz: 13.LQ_CH32V103_TestBluetooth.elf
	@	riscv-none-embed-size --format=berkeley "13.LQ_CH32V103_TestBluetooth.elf"
	@	@
# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) 13.LQ_CH32V103_TestBluetooth.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
