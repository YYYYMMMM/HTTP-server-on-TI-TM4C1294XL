#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = D:/TI/ccs/tirtos_tivac_2_16_01_14/packages;D:/TI/ccs/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages;D:/TI/ccs/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages;D:/TI/ccs/tirtos_tivac_2_16_01_14/products/ndk_2_25_00_09/packages;D:/TI/ccs/tirtos_tivac_2_16_01_14/products/uia_2_00_05_50/packages;D:/TI/ccs/tirtos_tivac_2_16_01_14/products/ns_1_11_00_10/packages;D:/TI/ccs/ccsv6/ccs_base;F:/Embedded_System/PROJECT/http_serverCC3100/wifi_node_ap_tirtos/.config
override XDCROOT = D:/TI/ccs/xdctools_3_31_03_43_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = D:/TI/ccs/tirtos_tivac_2_16_01_14/packages;D:/TI/ccs/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages;D:/TI/ccs/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages;D:/TI/ccs/tirtos_tivac_2_16_01_14/products/ndk_2_25_00_09/packages;D:/TI/ccs/tirtos_tivac_2_16_01_14/products/uia_2_00_05_50/packages;D:/TI/ccs/tirtos_tivac_2_16_01_14/products/ns_1_11_00_10/packages;D:/TI/ccs/ccsv6/ccs_base;F:/Embedded_System/PROJECT/http_serverCC3100/wifi_node_ap_tirtos/.config;D:/TI/ccs/xdctools_3_31_03_43_core/packages;..
HOSTOS = Windows
endif
