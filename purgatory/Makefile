#
# Purgatory (an uncomfortable intermediate state)
#            In this case the code that runs between kernels
#

# There is probably a cleaner way to do this but for now this
# should keep us from accidentially include unsafe library functions
# or headers.

PURGATORY = purgatory/purgatory.ro
PURGATORY_SRCS =
PURGATORY_SRCS += purgatory/purgatory.c
PURGATORY_SRCS += purgatory/printf.c
PURGATORY_SRCS += purgatory/string.c

DUMMY_PURGATORY = purgatory/dummy_purgatory.ro

dist += purgatory/Makefile $(PURGATORY_SRCS)				\
	purgatory/include/purgatory.h purgatory/include/string.h

include $(srcdir)/purgatory/arch/alpha/Makefile
include $(srcdir)/purgatory/arch/arm/Makefile
include $(srcdir)/purgatory/arch/i386/Makefile
include $(srcdir)/purgatory/arch/ia64/Makefile
include $(srcdir)/purgatory/arch/mips/Makefile
include $(srcdir)/purgatory/arch/ppc/Makefile
include $(srcdir)/purgatory/arch/ppc64/Makefile
include $(srcdir)/purgatory/arch/s390/Makefile
include $(srcdir)/purgatory/arch/sh/Makefile
include $(srcdir)/purgatory/arch/x86_64/Makefile

REAL_PURGATORY_SRCS+=$(PURGATORY_SRCS) $($(ARCH)_PURGATORY_SRCS) $($(ARCH)_PURGATORY_SETUP)
DUMMY_PURGATORY_SRCS+=$(PURGATORY_SRCS) $($(ARCH)_PURGATORY_SRCS) $($(ARCH)_DUMMY_PURGATORY_SETUP)

PURGATORY_OBJS = $(call objify, $(REAL_PURGATORY_SRCS)) purgatory/sha256.o
PURGATORY_DEPS = $(call depify, $(PURGATORY_OBJS))

DUMMY_PURGATORY_OBJS = $(call objify, $(DUMMY_PURGATORY_SRCS)) purgatory/sha256.o
DUMMY_PURGATORY_DEPS = $(call depify, $(DUMMY_PURGATORY_OBJS))

clean += $(PURGATORY_OBJS) $(PURGATORY_DEPS) $(PURGATORY) $(DUMMY_PURGATORY_OBJS) $(DUMMY_PURGATORY_DEPS) $(DUMMY_PURGATORY)

-include $(PURGATORY_DEPS)
-include $(DUMMY_PURGATORY_DEPS)

# sha256.c needs to be compiled without optimization, else
# purgatory fails to execute on ia64.
purgatory/sha256.o: CFLAGS += -O0

purgatory/sha256.o: $(srcdir)/util_lib/sha256.c
	mkdir -p $(@D)
	$(COMPILE.c) -o $@ $^

$(PURGATORY): CC=$(TARGET_CC)
$(PURGATORY): CFLAGS+=$(PURGATORY_EXTRA_CFLAGS) \
		      $($(ARCH)_PURGATORY_EXTRA_CFLAGS) \
		      -Os -fno-builtin -ffreestanding

$(PURGATORY): CPPFLAGS=$($(ARCH)_PURGATORY_EXTRA_CFLAGS) \
			-I$(srcdir)/purgatory/include \
			-I$(srcdir)/purgatory/arch/$(ARCH)/include \
			-I$(srcdir)/util_lib/include \
			-I$(srcdir)/include \
			-I$(shell $(CC) -print-file-name=include)
$(PURGATORY): LDFLAGS=$($(ARCH)_PURGATORY_EXTRA_CFLAGS)\
			-Wl,--no-undefined -nostartfiles -nostdlib \
			-nodefaultlibs -e purgatory_start -r

$(DUMMY_PURGATORY): CC=$(TARGET_CC)
$(DUMMY_PURGATORY): CFLAGS+=$(PURGATORY_EXTRA_CFLAGS) \
		      $($(ARCH)_PURGATORY_EXTRA_CFLAGS) \
		      -Os -fno-builtin -ffreestanding

$(DUMMY_PURGATORY): CPPFLAGS=$($(ARCH)_PURGATORY_EXTRA_CFLAGS) \
			-I$(srcdir)/purgatory/include \
			-I$(srcdir)/purgatory/arch/$(ARCH)/include \
			-I$(srcdir)/util_lib/include \
			-I$(srcdir)/include \
			-I$(shell $(CC) -print-file-name=include)
$(DUMMY_PURGATORY): LDFLAGS=$($(ARCH)_PURGATORY_EXTRA_CFLAGS)\
			-Wl,--no-undefined -nostartfiles -nostdlib \
			-nodefaultlibs -e dummy_purgatory_start -r

$(PURGATORY): $(PURGATORY_OBJS)
	$(MKDIR) -p $(@D)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

#	$(LD) $(LDFLAGS) $(EXTRA_LDFLAGS) --no-undefined -e purgatory_start -r -o $@ $(PURGATORY_OBJS) $(UTIL_LIB)
	$(STRIP) --strip-debug $@

$(DUMMY_PURGATORY): $(DUMMY_PURGATORY_OBJS)
	$(MKDIR) -p $(@D)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

#	$(LD) $(LDFLAGS) $(EXTRA_LDFLAGS) --no-undefined -e purgatory_start -r -o $@ $(PURGATORY_OBJS) $(UTIL_LIB)
	$(STRIP) --strip-debug $@

echo::
	@echo "PURGATORY_SRCS $(PURGATORY_SRCS)"
	@echo "PURGATORY_DEPS $(PURGATORY_DEPS)"
	@echo "PURGATORY_OBJS $(PURGATORY_OBJS)"
