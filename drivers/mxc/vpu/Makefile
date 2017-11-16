#
# Makefile for the VPU drivers.
#

obj-$(CONFIG_MXC_VPU)                  += mxc_vpu.o

ifeq ($(CONFIG_MXC_VPU_DEBUG),y)
EXTRA_CFLAGS += -DDEBUG
endif

ifeq ($(CONFIG_AIRTAME_MXC_VPU_PREALLOCATE),y)
EXTRA_CFLAGS+=-DAIRTAME_MXC_VPU_PREALLOCATE
endif