## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,em4f linker.cmd package/cfg/http_server_WIFI_demo_ap_TIRTOS_pem4f.oem4f

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/http_server_WIFI_demo_ap_TIRTOS_pem4f.xdl
	$(SED) 's"^\"\(package/cfg/http_server_WIFI_demo_ap_TIRTOS_pem4fcfg.cmd\)\"$""\"C:/ti/tirtos_tivac_2_10_01_38/tirtos_tivac_2_10_01_38_examples/TI/EK_TM4C1294XL/http_server _WIFI_demo_ap _TIRTOS/.config/xconfig_http_server_WIFI_demo_ap_TIRTOS/\1\""' package/cfg/http_server_WIFI_demo_ap_TIRTOS_pem4f.xdl > $@
	-$(SETDATE) -r:max package/cfg/http_server_WIFI_demo_ap_TIRTOS_pem4f.h compiler.opt compiler.opt.defs
