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
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

CFLAGS:=$(CFLAGS) -D__is_frog -Iinclude -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS)
LIBFROGK_CFLAGS:=$(CFLAGS) -D__is_kernel -Iinclude -ffreestanding -Wall -Wextra
LIBFROGK_CPPFLAGS:=$(CPPFLAGS) -fno-rtti -fno-exceptions

ARCHDIR=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
FROGK_CFLAGS:=$(FROGK_CFLAGS) $(KERNEL_ARCH_CFLAGS)
FROGK_CPPFLAGS:=$(FROGK_CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)

BUILDDIR=$(abspath build)

FREEOBJS=\
$(ARCH_FREEOBJS) \

HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \

OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \

FROGK_OBJS=$(FREEOBJS:.o=.libfrogk.o)

BINARIES=libfrogk.a

.PHONY: all always clean install install-headers install-libs
.SUFFIXES: .o .libfrogk.o .cpp .S

all: $(BINARIES)

frog.a: always $(OBJS)
	cd $(BUILDDIR) && $(AR) rcs $@ $(OBJS)

libfrogk.a: always $(LIBFROGK_OBJS)
	cd $(BUILDDIR) && $(AR) rcs $@ $(LIBFROGK_OBJS)

.cpp.o:
	$(CXX) -MD -c $< -o $(BUILDDIR)/$@ $(CFLAGS) $(CPPFLAGS)

.S.o:
	$(CXX) -MD -c $< -o $(BUILDDIR)/$@ $(CFLAGS) $(CPPFLAGS)

.cpp.libfrogk.o:
	$(CXX) -MD -c $< -o $(BUILDDIR)/$@ $(LIBFROGK_CFLAGS) $(LIBFROGK_CPPFLAGS)

.S.libfrogk.o:
	$(CXX) -MD -c $< -o $(BUILDDIR)/$@ $(LIBFROGK_CFLAGS) $(LIBFROGK_CPPFLAGS)

clean:
	rm -rf $(BUILDDIR)

always:
	mkdir -p $(BUILDDIR)

install: install-headers install-libs

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BUILDDIR)/$(BINARIES) $(DESTDIR)$(LIBDIR)

-include $(OBJS:.o=.d)
-include $(LIBFROGK_OBJS:.o=.d)