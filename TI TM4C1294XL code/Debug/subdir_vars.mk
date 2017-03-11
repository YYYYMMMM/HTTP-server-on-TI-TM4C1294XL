################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../tm4c1294ncpdt.cmd 

CFG_SRCS += \
../wifi_node_ap.cfg 

LIB_SRCS += \
../grlib.lib \
../usblib.lib 

C_SRCS += \
../EK_TM4C1294XL.c \
../board.c \
../buttons.c \
../cli_uart.c \
../io.c \
../main.c \
../spi.c \
../uart.c 

OBJS += \
./EK_TM4C1294XL.obj \
./board.obj \
./buttons.obj \
./cli_uart.obj \
./io.obj \
./main.obj \
./spi.obj \
./uart.obj 

C_DEPS += \
./EK_TM4C1294XL.d \
./board.d \
./buttons.d \
./cli_uart.d \
./io.d \
./main.d \
./spi.d \
./uart.d 

GEN_MISC_DIRS += \
./configPkg/ 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_OPTS += \
./configPkg/compiler.opt 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

C_DEPS__QUOTED += \
"EK_TM4C1294XL.d" \
"board.d" \
"buttons.d" \
"cli_uart.d" \
"io.d" \
"main.d" \
"spi.d" \
"uart.d" 

OBJS__QUOTED += \
"EK_TM4C1294XL.obj" \
"board.obj" \
"buttons.obj" \
"cli_uart.obj" \
"io.obj" \
"main.obj" \
"spi.obj" \
"uart.obj" 

C_SRCS__QUOTED += \
"../EK_TM4C1294XL.c" \
"../board.c" \
"../buttons.c" \
"../cli_uart.c" \
"../io.c" \
"../main.c" \
"../spi.c" \
"../uart.c" 


