ifndef TARGET
TARGET=z1
endif

all: terraZoo
APPS=serial-shell



WITH_UIP6=1
UIP_CONF_IPV6=1
CFLAGS+= -DUIP_CONF_IPV6_RPL


PROJECT_SOURCEFILES += xively_cst.c http_put.c

CONTIKI = ../..
include $(CONTIKI)/Makefile.include

