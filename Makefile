APPLICATION = IOT2023Project1

BOARD ?= iotlab-m3

USEMODULE += netdev_default
LWIP_IPV6 ?= 0

ifeq (,$(filter 1, $(LWIP_IPV4) $(LWIP_IPV6)))
  USEMODULE += auto_init_gnrc_netif
  # Specify the mandatory networking modules
  USEMODULE += gnrc_ipv6_default
  # Additional networking modules that can be dropped if not needed
  USEMODULE += gnrc_icmpv6_echo
else
  USEMODULE += lwip_netdev

endif

USEMODULE += gcoap
USEMODULE += ipv6_addr
USEMODULE += shell_commands
USEMODULE += ztimer_msec

USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_ipv6_default
USEMODULE += od
USEMODULE += fmt
USEMODULE += netutils
USEMODULE += random
USEMODULE += shell
USEMODULE += shell_cmds_default
USEMODULE += ps
USEMODULE += lsm303dlhc

RIOTBASE ?= $(CURDIR)/../RIOT
include $(RIOTBASE)/Makefile.include
