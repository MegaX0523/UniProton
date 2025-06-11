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

export AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP

TOPDIR := $(shell pwd)
ROOT_DIR := $(TOPDIR)/../../..
REAL_TIME_DIR := $(ROOT_DIR)/src/real_time
LIBCK_DIR := $(REAL_TIME_DIR)/libck
LIBCK_KERNEL_DIR := $(REAL_TIME_DIR)/libck/kernel
DRIVERS_DIR := $(REAL_TIME_DIR)/drivers
OSAL_DIR := $(ROOT_DIR)/platform/osal

OUTPUT_DIR := $(ROOT_DIR)/build/build_libck/output
LIBMETAL_OUTPUT_DIR := $(OUTPUT_DIR)/libmetal
OPENAMP_OUTPUT_DIR := $(OUTPUT_DIR)/openamp

CFLAGS   := -fno-builtin -ffreestanding \
	-I$(LIBMETAL_OUTPUT_DIR)/include \
	-I$(OPENAMP_OUTPUT_DIR)/include \
	-I$(LIBCK_DIR)/include \
	-I$(LIBCK_KERNEL_DIR)/libc/include \
	-I$(LIBCK_KERNEL_DIR)/include \
	-I$(LIBCK_KERNEL_DIR)/include/hw/cpu \
	-I$(LIBCK_KERNEL_DIR)/include/securec \
	-I$(LIBCK_KERNEL_DIR)/include/common\
	-I$(LIBCK_KERNEL_DIR)/config \
	-I$(LIBCK_KERNEL_DIR)/include/arch \
	-I$(LIBCK_KERNEL_DIR)/include/external \
	-I$(DRIVERS_DIR)/include/ \
	-I$(DRIVERS_DIR)/common/ \
	-I$(OUTPUT_DIR)/securec/include \
	-I$(OSAL_DIR)/include \
	-DTEXT_BASE=$(TEXTBASE) -O2 \
	-fno-pic -ffunction-sections \
	-fdata-sections -fno-common \
	-fno-common -pipe -march=armv8-a \
	-Wall -Wstrict-prototypes -fstack-protector-strong \
	$(MKFLAGS) -fno-strict-aliasing

CFLAGS += -Wdate-time -Wmissing-declarations -Wmissing-prototypes\
	-Wfloat-equal -Wformat=2  -Wswitch-default -Wtrampolines -Winit-self \
	-Wundef -Wimplicit-fallthrough=2 -Wshadow

POSIX_OPTION := -D_POSIX_THREADS -D_POSIX_THREAD_PRIORITY_SCHEDULING -D_POSIX_PRIORITY_SCHEDULING \
 				-D_POSIX_TIMERS -D_POSIX_CPUTIME -D_POSIX_THREAD_CPUTIME -D_POSIX_MONOTONIC_CLOCK -D_POSIX_TIMEOUTS \
  				-D_POSIX_CLOCK_SELECTION -D_POSIX_THREAD_PRIO_PROTECT -D_UNIX98_THREAD_MUTEX_ATTRIBUTES \
				-D_POSIX_READER_WRITER_LOCKS

CFLAGS += $(POSIX_OPTION)
CFLAGS += -D__LIBCK__
ASFLAGS:= $(CFLAGS)

ifneq ($(wildcard $(LIBCK_DIR)/kernel/lib/libHI3093.a),)
EXTEND_INCLUDE += $(LIBCK_DIR)/kernel/lib/libHI3093.a
else
$(error "can not find libHI3093.a")
endif

export CFLAGS LDFLAGS

TARGET := liblibck.a

obj-y += $(LIBCK_KERNEL_DIR)/config/sre_config.o
obj-y += bsp/
obj-y += mcs/
obj-y += $(DRIVERS_DIR)/
obj-y += $(OSAL_DIR)/libck/osal_interrupt.o
obj-y += $(OSAL_DIR)/libck/osal_semaphore.o
obj-y += $(OSAL_DIR)/libck/osal_debug.o

$(TARGET):
	make -C ./ -f Makefile.build MAKE_BUILD_DIR=$(LIBCK_DIR)
	pushd $(LIBCK_KERNEL_DIR)/lib; $(AR) x $(EXTEND_INCLUDE); popd
	$(AR) cr $(TARGET) built-in.o $(LIBCK_DIR)/kernel/lib/*.o
	$(RANLIB) $(TARGET)

install:$(TARGET)
	mkdir -p $(REAL_TIME_DIR)/lib
	cp $(TARGET) $(REAL_TIME_DIR)/lib

clean:
	rm -f $(shell find $(LIBCK_DIR) -name "*.o")
	rm -f $(shell find $(LIBCK_DIR) -name "*.d" ! -path "$(LIBCK_DIR)/kernel/*")
	rm -f $(shell find $(LIBCK_DIR) -name "*.a" ! -path "$(LIBCK_DIR)/kernel/*")
	rm -f $(shell find $(DRIVERS_DIR) -name "*.o")
	rm -f $(shell find $(OSAL_DIR)/libck -name "*.o")