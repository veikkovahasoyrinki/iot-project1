APPLICATION = IOT2023Project1

BOARD ?= iotlab-m3

USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ztimer_msec

USEMODULE += lsm303dlhc

RIOTBASE ?= $(CURDIR)/../RIOT-master

include $(RIOTBASE)/Makefile.include

