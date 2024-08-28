CONTIKI_PROJECT = udp-client udp-server border-router
all: $(CONTIKI_PROJECT)
CONTIKI=../..

include $(CONTIKI)/Makefile.dir-variables
MODULES += $(CONTIKI_NG_SERVICES_DIR)/rpl-border-router
# Include webserver module
MODULES_REL += webserver

include $(CONTIKI)/Makefile.include
