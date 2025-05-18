
ROOT_DIR:=$(shell dirname $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST)))))
$(info ===== Build Configuration [FROG] =====)
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

CFLAGS:=$(CFLAGS) -Iinclude -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS)
LIBFROGK_CFLAGS:=$(CFLAGS) -D__is_kernel -Iinclude -ffreestanding -Wall -Wextra -Werror=return-type
LIBFROGK_CPPFLAGS:=$(CPPFLAGS) -fno-rtti -fno-exceptions
 
ARCHDIR=arch/$(HOSTARCH)
$(info Architecture directory: $(ARCHDIR))

include $(ARCHDIR)/make.config
 
CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
LIBFROGK_CFLAGS:=$(LIBFROGK_CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBFROGK_CPPFLAGS:=$(LIBFROGK_CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
 
BUILDDIR=$(abspath build)
$(info Build directory: $(BUILDDIR))

FREEOBJS=			\
$(ARCH_FREEOBJS)	\
FROG/Memory.o		\
FROG/String.o		\
FROG/StringView.o	\
FROG/Time.o			\
 
HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \
 
OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \
 
LIBFROGK_OBJS=$(FREEOBJS:.o=.frogk.o)
 
BINARIES=libfrogk.a
 
.PHONY: all always clean install install-headers install-libs
.SUFFIXES: .o .frogk.o .cpp .S
 
all: $(BINARIES)
 
libfrog.a: always $(OBJS)
	cd $(BUILDDIR) && $(AR) rcs $@ $(OBJS)
 
libfrogk.a: always $(LIBFROGK_OBJS)
	cd $(BUILDDIR) && $(AR) rcs $@ $(LIBFROGK_OBJS)
 
.cpp.o:
	$(CXX) -MD -c $< -o $(BUILDDIR)/$@ $(CFLAGS) $(CPPFLAGS)
 
.S.o:
	$(CXX) -MD -c $< -o $(BUILDDIR)/$@ $(CFLAGS) $(CPPFLAGS)
 
.cpp.frogk.o:
	$(CXX) -MD -c $< -o $(BUILDDIR)/$@ $(LIBFROGK_CFLAGS) $(LIBFROGK_CPPFLAGS)
 
.S.frogk.o:
	$(CXX) -MD -c $< -o $(BUILDDIR)/$@ $(LIBFROGK_CFLAGS) $(LIBFROGK_CPPFLAGS)
 
clean:
	rm -rf $(BUILDDIR)
 
always:
	mkdir -p $(BUILDDIR)/FROG
 
install: install-headers install-libs
 
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.
 
install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BUILDDIR)/$(BINARIES) $(DESTDIR)$(LIBDIR)
 
-include $(OBJS:.o=.d)
-include $(LIBFROGK_OBJS:.o=.d)