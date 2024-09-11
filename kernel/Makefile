# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maroy <maroy@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/05 01:34:45 by mathroy0310       #+#    #+#              #
#    Updated: 2024/09/03 16:54:15 by maroy            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DEFAULT_HOST!=../default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)
 
CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=
 
DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include
 
CFLAGS:=$(CFLAGS) -D__is_kernel -D__arch=$(HOSTARCH) -Iinclude -fstack-protector -ffreestanding -Wall -Wextra -Wno-unused-function -Werror=return-type
CPPFLAGS:=$(CPPFLAGS) -fno-rtti -fno-exceptions -fno-omit-frame-pointer -mno-sse -mno-sse2
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -L$(SYSROOT)/usr/lib -lk -lfrogk -lgcc
 
ARCHDIR=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config
 
CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)
 
ifeq ($(UBSAN), 1)
CFLAGS:=$(CFLAGS) -fsanitize=undefined
endif

BUILDDIR=$(abspath build)
 
KERNEL_OBJS=					\
$(KERNEL_ARCH_OBJS)				\
font/prefs.o  					\
kernel/APIC.o					\
kernel/build_libc.o				\
kernel/CPUID.o					\
kernel/Debug.o 					\
kernel/Font.o					\
kernel/FS/Ext2.o				\
kernel/FS/Inode.o				\
kernel/FS/VirtualFileSystem.o	\
kernel/Input.o					\
kernel/InterruptController.o	\
kernel/kernel.o					\
kernel/kmalloc.o				\
kernel/PCI.o 					\
kernel/PIC.o					\
kernel/PIT.o					\
kernel/Process.o				\
kernel/RTC.o					\
kernel/Scheduler.o 				\
kernel/Serial.o					\
kernel/Shell.o					\
kernel/SpinLock.o 				\
kernel/SSP.o					\
kernel/Storage/ATAController.o	\
kernel/Storage/StorageDevice.o	\
kernel/Syscall.o				\
kernel/Thread.o 				\
kernel/TTY.o					\
kernel/VesaTerminalDriver.o		\
userspace/userspace.o			\
icxxabi.o						\
ubsan.o							\
 
OBJS=					\
$(ARCHDIR)/crti.o		\
$(ARCHDIR)/crtbegin.o	\
$(KERNEL_OBJS)			\
$(ARCHDIR)/crtend.o		\
$(ARCHDIR)/crtn.o		\
 
LINK_LIST=				\
$(LDFLAGS)				\
$(ARCHDIR)/crti.o		\
$(ARCHDIR)/crtbegin.o	\
$(KERNEL_OBJS)			\
$(LIBS)					\
$(ARCHDIR)/crtend.o		\
$(ARCHDIR)/crtn.o		\
 
.PHONY: all always clean install install-headers install-kernel
.SUFFIXES: .o .c .cpp .S .psf
 
all: frog-os.kernel
 
frog-os.kernel: always $(OBJS) $(ARCHDIR)/linker.ld
	cd $(BUILDDIR) && $(CXX) -T ../$(ARCHDIR)/linker.ld -o frog-os.kernel $(CFLAGS) $(CPPFLAGS) $(LINK_LIST)
	cd $(BUILDDIR) && grub-file --is-x86-multiboot frog-os.kernel
 
$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $(BUILDDIR)/$@
 
.c.o:
	$(CC) -MD -c $< -o $(BUILDDIR)/$@ $(CFLAGS)

.cpp.o:
	$(CXX) -MD -c $< -o $(BUILDDIR)/$@ $(CFLAGS) $(CPPFLAGS)
 
.S.o:
	$(CC) -MD -c $< -o $(BUILDDIR)/$@ $(CFLAGS)
 
.psf.o:
	objcopy -O $(ELF_FORMAT) -B i386 -I binary $< $(BUILDDIR)/$@
 
always:
	mkdir -p $(BUILDDIR)/$(ARCHDIR)
	mkdir -p $(BUILDDIR)/kernel
	mkdir -p $(BUILDDIR)/kernel/FS
	mkdir -p $(BUILDDIR)/kernel/Storage
	mkdir -p $(BUILDDIR)/userspace
	mkdir -p $(BUILDDIR)/font

clean:
	rm -rf $(BUILDDIR)

install: install-headers install-kernel
 
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.
 
install-kernel: frog-os.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp $(BUILDDIR)/frog-os.kernel $(DESTDIR)$(BOOTDIR)
 
-include $(OBJS:.o=.d)