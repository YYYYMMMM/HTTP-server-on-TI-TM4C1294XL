################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5

GEN_OPTS__FLAG := --cmd_file="configPkg/compiler.opt"
GEN_CMDS__FLAG := -l"configPkg/linker.cmd"

ORDERED_OBJS += \
"./EK_TM4C1294XL.obj" \
"./board.obj" \
"./buttons.obj" \
"./cli_uart.obj" \
"./io.obj" \
"./main.obj" \
"./spi.obj" \
"./uart.obj" \
"./simplelink/source/device.obj" \
"./simplelink/source/driver.obj" \
"./simplelink/source/flowcont.obj" \
"./simplelink/source/fs.obj" \
"./simplelink/source/netapp.obj" \
"./simplelink/source/netcfg.obj" \
"./simplelink/source/nonos.obj" \
"./simplelink/source/socket.obj" \
"./simplelink/source/spawn.obj" \
"./simplelink/source/wlan.obj" \
"../grlib.lib" \
"../tm4c1294ncpdt.cmd" \
"../usblib.lib" \
$(GEN_CMDS__FLAG) \
-l"D:/TI/TivaWare_C_Series-2.1.2.111/grlib/ccs/Debug/grlib.lib" \
-l"D:/TI/TivaWare_C_Series-2.1.2.111/usblib/ccs/Debug/usblib.lib" \
-l"D:/TI/TivaWare_C_Series-2.1.2.111/driverlib/ccs/Debug/driverlib.lib" \
-l"D:/TI/TivaWare_C_Series-2.1.2.111/sensorlib/ccs/Debug/sensorlib.lib" \
-l"D:/TI/ccs/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b/driverlib/ccs/Debug/driverlib.lib" \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include simplelink/source/subdir_vars.mk
-include subdir_rules.mk
-include simplelink/source/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
wifi_node_ap_tirtos.out \

EXE_OUTPUTS__QUOTED += \
"wifi_node_ap_tirtos.out" \

BIN_OUTPUTS += \
wifi_node_ap_tirtos.hex \

BIN_OUTPUTS__QUOTED += \
"wifi_node_ap_tirtos.hex" \


# All Target
all: wifi_node_ap_tirtos.out

# Tool invocations
wifi_node_ap_tirtos.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --opt_for_speed=2 -g --gcc --define=PART_TM4C1294NCPDT --define=TARGET_IS_TM4C129_RA1 --define=_USE_CLI_ --define=ccs="ccs" --define="TIRTOS" --diag_wrap=off --diag_warning=225 --display_error_number -z -m"wifi_node_ap_tirtos.map" --heap_size=0 --stack_size=700 -i"D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/lib" -i"D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --reread_libs --display_error_number --diag_wrap=off --warn_sections --xml_link_info="wifi_node_ap_tirtos_linkInfo.xml" --rom_model -o "wifi_node_ap_tirtos.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

wifi_node_ap_tirtos.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: ARM Hex Utility'
	"D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armhex"  -o "wifi_node_ap_tirtos.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_OPTS__QUOTED)$(GEN_CMDS__QUOTED)
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-$(RM) "EK_TM4C1294XL.d" "board.d" "buttons.d" "cli_uart.d" "io.d" "main.d" "spi.d" "uart.d" "simplelink\source\device.d" "simplelink\source\driver.d" "simplelink\source\flowcont.d" "simplelink\source\fs.d" "simplelink\source\netapp.d" "simplelink\source\netcfg.d" "simplelink\source\nonos.d" "simplelink\source\socket.d" "simplelink\source\spawn.d" "simplelink\source\wlan.d" 
	-$(RM) "EK_TM4C1294XL.obj" "board.obj" "buttons.obj" "cli_uart.obj" "io.obj" "main.obj" "spi.obj" "uart.obj" "simplelink\source\device.obj" "simplelink\source\driver.obj" "simplelink\source\flowcont.obj" "simplelink\source\fs.obj" "simplelink\source\netapp.obj" "simplelink\source\netcfg.obj" "simplelink\source\nonos.obj" "simplelink\source\socket.obj" "simplelink\source\spawn.obj" "simplelink\source\wlan.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

