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


ifndef CONFIG_KCONFIG_MODULE_GCOAP
## Uncomment to redefine port, for example use 61616 for RFC 6282 UDP compression.
#GCOAP_PORT = 5683
#CFLAGS += -DCONFIG_GCOAP_PORT=$(GCOAP_PORT)

## Uncomment to redefine request token length, max 8.
#GCOAP_TOKENLEN = 2
#CFLAGS += -DCONFIG_GCOAP_TOKENLEN=$(GCOAP_TOKENLEN)

# Increase from default for confirmable block2 follow-on requests
GCOAP_RESEND_BUFS_MAX ?= 2
CFLAGS += -DCONFIG_GCOAP_RESEND_BUFS_MAX=$(GCOAP_RESEND_BUFS_MAX)
endif








