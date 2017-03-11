# invoke SourceDir generated makefile for wifi_node_ap.pem4f
wifi_node_ap.pem4f: .libraries,wifi_node_ap.pem4f
.libraries,wifi_node_ap.pem4f: package/cfg/wifi_node_ap_pem4f.xdl
	$(MAKE) -f F:\Embedded_System\PROJECT\http_serverCC3100\wifi_node_ap_tirtos/src/makefile.libs

clean::
	$(MAKE) -f F:\Embedded_System\PROJECT\http_serverCC3100\wifi_node_ap_tirtos/src/makefile.libs clean

