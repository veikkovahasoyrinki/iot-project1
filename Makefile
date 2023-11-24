APPLICATION = IOT2023Project1

BOARD ?= iotlab-m3

USEMODULE += ipv6_addr
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ztimer_msec

USEMODULE += gcoap
USEMODULE += auto_init_gnrc_netif
  # Specify the mandatory networking modules
USEMODULE += gnrc_ipv6_default
USEMODULE += od
USEMODULE += fmt
USEMODULE += netutils
USEMODULE += random
# Add also the shell, some shell commands
USEMODULE += shell
USEMODULE += shell_cmds_default
USEMODULE += ps
USEMODULE += lsm303dlhc

RIOTBASE ?= $(CURDIR)/../RIOT

include $(RIOTBASE)/Makefile.include

