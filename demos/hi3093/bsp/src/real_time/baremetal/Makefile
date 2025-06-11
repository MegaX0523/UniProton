AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm

STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump
RANLIB		= $(CROSS_COMPILE)ranlib

export AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP RANLIB

TOPDIR := $(shell pwd)
ROOT_DIR := $(TOPDIR)/../../..
REAL_TIME_DIR := $(ROOT_DIR)/src/real_time
BAREMETAL_DIR := $(REAL_TIME_DIR)/baremetal
DRIVERS_DIR := $(REAL_TIME_DIR)/drivers
UBOOT_DIR = $(ROOT_DIR)/open_source/u-boot/u-boot
OSAL_DIR := $(ROOT_DIR)/platform/osal

OUTPUT_DIR := $(ROOT_DIR)/build/build_baremetal/output
LIBMETAL_OUTPUT_DIR := $(OUTPUT_DIR)/libmetal
OPENAMP_OUTPUT_DIR := $(OUTPUT_DIR)/openamp

CFLAGS   := -fno-builtin -ffreestanding \
	-I$(LIBMETAL_OUTPUT_DIR)/include \
	-I$(OPENAMP_OUTPUT_DIR)/include \
	-I$(BAREMETAL_DIR)/include \
	-I$(BAREMETAL_DIR)/memory \
	-I$(DRIVERS_DIR)/include \
	-I$(DRIVERS_DIR)/common \
	-I$(SECUREC_PATH)/include \
	-I$(OSAL_DIR)/include \
	-DTEXT_BASE=$(TEXTBASE) -Werror -O2 \
	-fno-pic -ffunction-sections \
	-fdata-sections -fno-common \
	-fno-common -pipe -march=armv8-a \
	-Wall -Wstrict-prototypes -fstack-protector-strong \
	$(MKFLAGS) -fno-strict-aliasing

CFLAGS += -Wdate-time -Wmissing-declarations -Wmissing-prototypes\
	-Wfloat-equal -Wformat=2  -Wswitch-default -Wtrampolines -Winit-self \
	-Wundef -Wimplicit-fallthrough=2 -Wshadow

CFLAGS += -D__NONOS__
ASFLAGS:= $(CFLAGS)

ifeq ($(MCS_2WIHT2_HI309X),y)
	CFLAGS+=-DMCS_2WIHT2_HI309X=y
endif

export CFLAGS LDFLAGS

TARGET := libbaremetal.a

obj-y += loader/
obj-y += message/
obj-y += hwi/
obj-y += osal_adapter/
obj-$(MCS_2WIHT2_HI309X) += mcs/
obj-$(MCS_2WIHT2_HI309X) += memory/
obj-y += $(DRIVERS_DIR)/
obj-y += $(OSAL_DIR)/nonos/osal_interrupt.o
obj-y += $(OSAL_DIR)/nonos/osal_semaphore.o

$(TARGET):
	make -C ./ -f Makefile.build MAKE_BUILD_DIR=$(BAREMETAL_DIR)
	$(AR) cr $(TARGET) built-in.o
	$(RANLIB) $(TARGET)

clean:
	rm -f $(shell find $(BAREMETAL_DIR) -name "*.o")
	rm -f $(shell find $(BAREMETAL_DIR) -name "*.a")
	rm -f $(shell find $(DRIVERS_DIR) -name "*.o")
	rm -f $(shell find $(DRIVERS_DIR) -name "*.i")
	rm -f $(shell find $(BAREMETAL_DIR) -name "*.i")
	rm -f $(shell find $(OSAL_DIR)/nonos -name "*.o")