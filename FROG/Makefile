# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maroy <maroy@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/05 01:17:18 by mathroy0310       #+#    #+#              #
#    Updated: 2024/08/18 00:05:13 by maroy            ###   ########.fr        #
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
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib
 
CFLAGS:=$(CFLAGS) -Iinclude -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS)
LIBFROGK_CFLAGS:=$(CFLAGS) -D__is_kernel -Iinclude -ffreestanding -Wall -Wextra -Werror=return-type
LIBFROGK_CPPFLAGS:=$(CPPFLAGS) -fno-rtti -fno-exceptions
 
ARCHDIR=arch/$(HOSTARCH)
 
include $(ARCHDIR)/make.config
 
CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
LIBFROGK_CFLAGS:=$(LIBFROGK_CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBFROGK_CPPFLAGS:=$(LIBFROGK_CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
 
BUILDDIR=$(abspath build)
 
FREEOBJS=			\
$(ARCH_FREEOBJS)	\
FROG/Memory.o		\
FROG/String.o		\
FROG/StringView.o	\
 
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