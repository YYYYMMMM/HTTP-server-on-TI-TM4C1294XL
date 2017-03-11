################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
EK_TM4C1294XL.obj: ../EK_TM4C1294XL.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --opt_for_speed=2 --include_path="D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix" --include_path="F:/Embedded_System/PROJECT/http_serverCC3100/wifi_node_ap_tirtos" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111/examples/boards/ek-tm4c1294xl-boostxl-senshub/drivers" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111/examples/boards/ek-tm4c1294xl-boostxl-senshub" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/source" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b/" --include_path="F:/Embedded_System/PROJECT/http_serverCC3100/wifi_node_ap_tirtos/" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/source" -g --gcc --define=PART_TM4C1294NCPDT --define=TARGET_IS_TM4C129_RA1 --define=_USE_CLI_ --define=ccs="ccs" --define="TIRTOS" --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="EK_TM4C1294XL.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

board.obj: ../board.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --opt_for_speed=2 --include_path="D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix" --include_path="F:/Embedded_System/PROJECT/http_serverCC3100/wifi_node_ap_tirtos" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111/examples/boards/ek-tm4c1294xl-boostxl-senshub/drivers" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111/examples/boards/ek-tm4c1294xl-boostxl-senshub" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/source" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b/" --include_path="F:/Embedded_System/PROJECT/http_serverCC3100/wifi_node_ap_tirtos/" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/source" -g --gcc --define=PART_TM4C1294NCPDT --define=TARGET_IS_TM4C129_RA1 --define=_USE_CLI_ --define=ccs="ccs" --define="TIRTOS" --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="board.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

buttons.obj: ../buttons.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --opt_for_speed=2 --include_path="D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix" --include_path="F:/Embedded_System/PROJECT/http_serverCC3100/wifi_node_ap_tirtos" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111/examples/boards/ek-tm4c1294xl-boostxl-senshub/drivers" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111/examples/boards/ek-tm4c1294xl-boostxl-senshub" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/source" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b/" --include_path="F:/Embedded_System/PROJECT/http_serverCC3100/wifi_node_ap_tirtos/" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/source" -g --gcc --define=PART_TM4C1294NCPDT --define=TARGET_IS_TM4C129_RA1 --define=_USE_CLI_ --define=ccs="ccs" --define="TIRTOS" --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="buttons.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

cli_uart.obj: ../cli_uart.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --opt_for_speed=2 --include_path="D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix" --include_path="F:/Embedded_System/PROJECT/http_serverCC3100/wifi_node_ap_tirtos" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111/examples/boards/ek-tm4c1294xl-boostxl-senshub/drivers" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111/examples/boards/ek-tm4c1294xl-boostxl-senshub" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/source" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b/" --include_path="F:/Embedded_System/PROJECT/http_serverCC3100/wifi_node_ap_tirtos/" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/source" -g --gcc --define=PART_TM4C1294NCPDT --define=TARGET_IS_TM4C129_RA1 --define=_USE_CLI_ --define=ccs="ccs" --define="TIRTOS" --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="cli_uart.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

io.obj: ../io.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --opt_for_speed=2 --include_path="D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix" --include_path="F:/Embedded_System/PROJECT/http_serverCC3100/wifi_node_ap_tirtos" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111/examples/boards/ek-tm4c1294xl-boostxl-senshub/drivers" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111/examples/boards/ek-tm4c1294xl-boostxl-senshub" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/source" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b/" --include_path="F:/Embedded_System/PROJECT/http_serverCC3100/wifi_node_ap_tirtos/" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/source" -g --gcc --define=PART_TM4C1294NCPDT --define=TARGET_IS_TM4C129_RA1 --define=_USE_CLI_ --define=ccs="ccs" --define="TIRTOS" --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="io.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --opt_for_speed=2 --include_path="D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix" --include_path="F:/Embedded_System/PROJECT/http_serverCC3100/wifi_node_ap_tirtos" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111/examples/boards/ek-tm4c1294xl-boostxl-senshub/drivers" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111/examples/boards/ek-tm4c1294xl-boostxl-senshub" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/source" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b/" --include_path="F:/Embedded_System/PROJECT/http_serverCC3100/wifi_node_ap_tirtos/" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/source" -g --gcc --define=PART_TM4C1294NCPDT --define=TARGET_IS_TM4C129_RA1 --define=_USE_CLI_ --define=ccs="ccs" --define="TIRTOS" --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

spi.obj: ../spi.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --opt_for_speed=2 --include_path="D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix" --include_path="F:/Embedded_System/PROJECT/http_serverCC3100/wifi_node_ap_tirtos" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111/examples/boards/ek-tm4c1294xl-boostxl-senshub/drivers" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111/examples/boards/ek-tm4c1294xl-boostxl-senshub" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/source" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b/" --include_path="F:/Embedded_System/PROJECT/http_serverCC3100/wifi_node_ap_tirtos/" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/source" -g --gcc --define=PART_TM4C1294NCPDT --define=TARGET_IS_TM4C129_RA1 --define=_USE_CLI_ --define=ccs="ccs" --define="TIRTOS" --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="spi.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

uart.obj: ../uart.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --opt_for_speed=2 --include_path="D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix" --include_path="F:/Embedded_System/PROJECT/http_serverCC3100/wifi_node_ap_tirtos" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111/examples/boards/ek-tm4c1294xl-boostxl-senshub/drivers" --include_path="D:/TI/TivaWare_C_Series-2.1.2.111/examples/boards/ek-tm4c1294xl-boostxl-senshub" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/source" --include_path="D:/TI/ccs/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b/" --include_path="F:/Embedded_System/PROJECT/http_serverCC3100/wifi_node_ap_tirtos/" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" --include_path="D:/TI/CC3100SDK_1.2.0/cc3100-sdk/simplelink/source" -g --gcc --define=PART_TM4C1294NCPDT --define=TARGET_IS_TM4C129_RA1 --define=_USE_CLI_ --define=ccs="ccs" --define="TIRTOS" --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="uart.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/linker.cmd: ../wifi_node_ap.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"D:/TI/ccs/xdctools_3_32_00_06_core/xs" --xdcpath="D:/TI/ccs/tirtos_tivac_2_16_01_14/packages;D:/TI/ccs/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages;D:/TI/ccs/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages;D:/TI/ccs/tirtos_tivac_2_16_01_14/products/ndk_2_25_00_09/packages;D:/TI/ccs/tirtos_tivac_2_16_01_14/products/uia_2_00_05_50/packages;D:/TI/ccs/tirtos_tivac_2_16_01_14/products/ns_1_11_00_10/packages;D:/TI/ccs/ccsv6/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.tiva:TM4C1294NCPDT -r debug -c "D:/TI/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.5" "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/compiler.opt: | configPkg/linker.cmd
configPkg/: | configPkg/linker.cmd


