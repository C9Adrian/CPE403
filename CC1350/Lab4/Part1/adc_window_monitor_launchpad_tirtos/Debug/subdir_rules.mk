################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
main_tirtos.obj: C:/ti/scs_academy/adc_window_monitor_launchpad/source/main_tirtos.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M3 --code_state=16 -me --include_path="C:/Users/Adrian/workspace_v8/adc_window_monitor_launchpad_tirtos" --include_path="C:/ti/simplelink_cc13x0_sdk_2_30_00_20/simplelink_cc13x0_sdk_2_30_00_20/source/ti/devices/cc13x0" --include_path="C:/ti/simplelink_cc13x0_sdk_2_30_00_20/simplelink_cc13x0_sdk_2_30_00_20/kernel/tirtos" --include_path="C:/ti/simplelink_cc13x0_sdk_2_30_00_20/simplelink_cc13x0_sdk_2_30_00_20/source" --include_path="C:/ti/simplelink_cc13x0_sdk_2_30_00_20/simplelink_cc13x0_sdk_2_30_00_20/source/ti/boards/CC1350_LAUNCHXL" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=DEVICE_FAMILY=cc13x0 --define=DeviceFamily_CC13X0 -g --c99 --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1873469023:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-1873469023-inproc

build-1873469023-inproc: ../rom.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_50_08_24_core/xs" --xdcpath="C:/ti/simplelink_cc13x0_sdk_2_30_00_20/source;C:/ti/simplelink_cc13x0_sdk_2_30_00_20/kernel/tirtos/packages;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M3 -p ti.platforms.simplelink:CC1350F128 -r release -c "C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS" --compileOptions "-mv7M3 --code_state=16 -me --include_path=\"C:/Users/Adrian/workspace_v8/adc_window_monitor_launchpad_tirtos\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_2_30_00_20/simplelink_cc13x0_sdk_2_30_00_20/source/ti/devices/cc13x0\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_2_30_00_20/simplelink_cc13x0_sdk_2_30_00_20/kernel/tirtos\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_2_30_00_20/simplelink_cc13x0_sdk_2_30_00_20/source\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_2_30_00_20/simplelink_cc13x0_sdk_2_30_00_20/source/ti/boards/CC1350_LAUNCHXL\" --include_path=\"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include\" --define=DEVICE_FAMILY=cc13x0 --define=DeviceFamily_CC13X0 -g --c99 --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi  " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-1873469023 ../rom.cfg
configPkg/compiler.opt: build-1873469023
configPkg/: build-1873469023

scif.obj: C:/ti/scs_academy/adc_window_monitor_launchpad/source/scif.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M3 --code_state=16 -me --include_path="C:/Users/Adrian/workspace_v8/adc_window_monitor_launchpad_tirtos" --include_path="C:/ti/simplelink_cc13x0_sdk_2_30_00_20/simplelink_cc13x0_sdk_2_30_00_20/source/ti/devices/cc13x0" --include_path="C:/ti/simplelink_cc13x0_sdk_2_30_00_20/simplelink_cc13x0_sdk_2_30_00_20/kernel/tirtos" --include_path="C:/ti/simplelink_cc13x0_sdk_2_30_00_20/simplelink_cc13x0_sdk_2_30_00_20/source" --include_path="C:/ti/simplelink_cc13x0_sdk_2_30_00_20/simplelink_cc13x0_sdk_2_30_00_20/source/ti/boards/CC1350_LAUNCHXL" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=DEVICE_FAMILY=cc13x0 --define=DeviceFamily_CC13X0 -g --c99 --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

scif_framework.obj: C:/ti/scs_academy/adc_window_monitor_launchpad/source/scif_framework.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M3 --code_state=16 -me --include_path="C:/Users/Adrian/workspace_v8/adc_window_monitor_launchpad_tirtos" --include_path="C:/ti/simplelink_cc13x0_sdk_2_30_00_20/simplelink_cc13x0_sdk_2_30_00_20/source/ti/devices/cc13x0" --include_path="C:/ti/simplelink_cc13x0_sdk_2_30_00_20/simplelink_cc13x0_sdk_2_30_00_20/kernel/tirtos" --include_path="C:/ti/simplelink_cc13x0_sdk_2_30_00_20/simplelink_cc13x0_sdk_2_30_00_20/source" --include_path="C:/ti/simplelink_cc13x0_sdk_2_30_00_20/simplelink_cc13x0_sdk_2_30_00_20/source/ti/boards/CC1350_LAUNCHXL" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=DEVICE_FAMILY=cc13x0 --define=DeviceFamily_CC13X0 -g --c99 --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


