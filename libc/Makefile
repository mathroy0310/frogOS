
ROOT_DIR:=$(shell dirname $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST)))))
$(info ===== Build Configuration [LIBC] =====)
$(info Root directory: $(ROOT_DIR))

DEFAULT_HOST:=$(shell sh $(ROOT_DIR)/default-host.sh)
$(info Using default-host.sh from: $(DEFAULT_HOST))

HOST?=DEFAULT_HOST
$(info HOST set to: $(HOST))

HOSTARCH:=$(shell sh $(ROOT_DIR)/target-triplet-to-arch.sh $(HOST))
$(info Host architecture: $(HOSTARCH))
 
CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=
 
DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib
 
$(info CFLAGS: $(CFLAGS))
$(info DESTDIR: $(DESTDIR))
$(info PREFIX: $(PREFIX))

CFLAGS:=$(CFLAGS) -D__is_libc -Iinclude -ffreestanding -Wall -Wextra -Werror=return-type
CPPFLAGS:=$(CPPFLAGS)
LIBK_CFLAGS:=$(CFLAGS) -D__is_libk
LIBK_CPPFLAGS:=$(CPPFLAGS)
 
ARCHDIR=arch/$(HOSTARCH)
$(info Architecture directory: $(ARCHDIR))

include $(ARCHDIR)/make.config
 
CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
LIBK_CFLAGS:=$(LIBK_CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBK_CPPFLAGS:=$(LIBK_CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
 
BUILDDIR=$(abspath build)
$(info Build directory: $(BUILDDIR))

FREEOBJS=			\
$(ARCH_FREEOBJS)	\
ctype.o             \
stdio/printf.o		\
stdio/putchar.o		\
stdio/puts.o		\
stdlib/abort.o		\
string/memcmp.o		\
string/memcpy.o		\
string/memmove.o	\
string/memset.o		\
string/strcmp.o 	\
string/strcpy.o		\
string/strerror.o   \
string/strlen.o		\
string/strncmp.o 	\
string/strncpy.o	\
string/strstr.o		\
 
HOSTEDOBJS=			\
$(ARCH_HOSTEDOBJS)	\
 
OBJS=				\
$(FREEOBJS)			\
$(HOSTEDOBJS)		\
 
LIBK_OBJS=$(FREEOBJS:.o=.libk.o)
 
#BINARIES=libc.a libk.a # Not ready for libc yet.
BINARIES=libk.a
 
.PHONY: all always clean install install-headers install-libs
.SUFFIXES: .o .libk.o .cpp .S
 
all: $(BINARIES)

libc.a: always $(OBJS)
	cd $(BUILDDIR) && $(AR) rcs $@ $(OBJS)

libk.a: always $(LIBK_OBJS)
	cd $(BUILDDIR) && $(AR) rcs $@ $(LIBK_OBJS)

.cpp.o:
	$(CXX) -MD -c $< -o $(BUILDDIR)/$@ $(CFLAGS) $(CPPFLAGS)
 
.S.o:
	$(CXX) -MD -c $< -o $(BUILDDIR)/$@ $(CFLAGS) $(CPPFLAGS)
 
.cpp.libk.o:
	$(CXX) -MD -c $< -o $(BUILDDIR)/$@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)
 
.S.libk.o:
	$(CXX) -MD -c $< -o $(BUILDDIR)/$@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)
 
clean:
	rm -rf $(BUILDDIR)
 
always:
	mkdir -p $(BUILDDIR)/stdio
	mkdir -p $(BUILDDIR)/stdlib
	mkdir -p $(BUILDDIR)/string
 
install: install-headers install-libs
 
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.
 
install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BUILDDIR)/$(BINARIES) $(DESTDIR)$(LIBDIR)
 
-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)