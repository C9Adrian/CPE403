#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/CCStudio_v6.0/ccsv6/ccs_base;C:/TI/tirtos_tivac_2_10_01_38/packages;C:/TI/tirtos_tivac_2_10_01_38/products/bios_6_41_00_26/packages;C:/TI/tirtos_tivac_2_10_01_38/products/ndk_2_24_01_18/packages;C:/TI/tirtos_tivac_2_10_01_38/products/uia_2_00_02_39/packages;C:/Users/a0159877/workspace_v6_0_ORIG_EAW_TTO/blink_TM4C_CLK/.config
override XDCROOT = C:/CCStudio_v6.0/xdctools_3_30_05_60_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/CCStudio_v6.0/ccsv6/ccs_base;C:/TI/tirtos_tivac_2_10_01_38/packages;C:/TI/tirtos_tivac_2_10_01_38/products/bios_6_41_00_26/packages;C:/TI/tirtos_tivac_2_10_01_38/products/ndk_2_24_01_18/packages;C:/TI/tirtos_tivac_2_10_01_38/products/uia_2_00_02_39/packages;C:/Users/a0159877/workspace_v6_0_ORIG_EAW_TTO/blink_TM4C_CLK/.config;C:/CCStudio_v6.0/xdctools_3_30_05_60_core/packages;..
HOSTOS = Windows
endif
