ifndef TARGET
TARGET=z1
endif

all: terraZoo
APPS=serial-shell



WITH_UIP6=1
UIP_CONF_IPV6=1
CFLAGS+= -DUIP_CONF_IPV6_RPL
#CFLAGS+= -DDEBUG
#CFLAGS+= -O3


PROJECT_SOURCEFILES += xively_cst.c http_put.c regulation.c in_out.c getlight.c gettmp.c

CONTIKI = ../..
include $(CONTIKI)/Makefile.include

