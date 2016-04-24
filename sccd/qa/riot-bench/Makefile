# name of your application
APPLICATION = hello-world

# If no BOARD is found in the environment, use this default:
BOARD ?= samr21-xpro

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../3rdparty/RIOT

USEMODULE += ps
USEMODULE += xtimer

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
SCCD_FLAGS_TOOL=$(CURDIR)/../../../flags.sh
SCCD_CCFLAGS=$(shell ${SCCD_FLAGS_TOOL} c25519 CCFLAGS) $(shell ${SCCD_FLAGS_TOOL} sccd CCFLAGS)

CFLAGS += -DDEVELHELP -I$(CURDIR)/../../.. $(SCCD_CCFLAGS) 

CFLAGS += "-DTHREAD_STACKSIZE_MAIN=6000"
CFLAGS += -Wno-unused-but-set-variable
CFLAGS += -Wno-error=unused-but-set-variable

LINKFLAGS += -lsccd_all

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
