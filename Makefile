ALL_SRC       := $(shell find src -type f -iname '*.c')
DEPS          := $(ALL_SRC:%.c=%.d)

CHECKVERS     := xbps-repo-checkvers
CHECKVERS_SRC := $(shell find src/checkvers -type f -iname '*.c')
CHECKVERS_OBJ := $(CHECKVERS_SRC:%.c=%.o)

COMMON_SRC    := $(shell find src/common -type f -iname '*.c')
COMMON_OBJ    := $(COMMON_SRC:%.c=%.o)

INC           := include

# FIXME: Update pkgconfig file for libxbps
EXTRA_PKGS  += proplib libarchive libconfuse
PKGS        += glib-2.0 libxbps $(EXTRA_PKGS)

STC         := -Wl,-Bstatic
DYN         := -Wl,-Bdynamic

NOSTATIC    +=

ifneq ($(NOSTATIC),)
  STC :=
  DYN :=
endif

PKG_CFLAGS  += $(shell pkg-config --cflags $(PKGS))
PKG_LDFLAGS += $(STC) $(shell pkg-config --libs --static $(PKGS)) $(DYN)

PKG_LDFLAGS := $(subst -pthread ,,$(PKG_LDFLAGS))
PKG_LDFLAGS := $(subst -lpthread ,,$(PKG_LDFLAGS))

# FIXME: Update pkgconfig file for libxbps
PKG_LDFLAGS := $(subst -lxbps,-lxbps -lfetch -lssl,$(PKG_LDFLAGS))

NODEPS      := clean

GCC_WARN    += -Wunsafe-loop-optimizations \
	       -Wdouble-promotion \
	       -Wsync-nand \
	       -Wsuggest-attribute=pure \
	       -Wsuggest-attribute=const \
	       -Wsuggest-attribute=noreturn \
	       -Wtrampolines \
	       -Wjump-misses-init \
	       -Wlogical-op \
	       -Wvector-operation-performance \
	       -Wunsuffixed-float-constants

WARN        += -Wall \
	       -Wextra \
	       -Werror \
	       -pedantic \
	       -pedantic-errors \
	       -Wformat=2 \
	       -Wformat-y2k \
	       -Wformat-nonliteral \
	       -Wformat-security \
	       -Winit-self \
	       -Wmissing-include-dirs \
	       -Wswitch-default \
	       -Wswitch-enum \
	       -Wmissing-format-attribute \
	       -Wfloat-equal \
	       -Wdeclaration-after-statement \
	       -Wundef \
	       -Wshadow \
	       -Wbad-function-cast \
	       -Wcast-qual \
	       -Wcast-align \
	       -Wwrite-strings \
	       -Wconversion \
	       -Waggregate-return \
	       -Wstrict-prototypes \
	       -Wold-style-definition \
	       -Wmissing-prototypes \
	       -Wmissing-declarations \
	       -Wpacked \
	       -Wredundant-decls \
	       -Wnested-externs \
	       -Winline \
	       -Wdisabled-optimization \
	       -fstack-protector -Wstack-protector \
	       -Woverlength-strings

OPT         += -O2 -pipe -mtune=generic -fPIC

GCC_CFLAGS  += --param ssp-buffer-size=4
CFLAGS      += -ansi $(OPT) $(PKG_CFLAGS)

DEFS        += -D_XOPEN_SOURCE=700 \
	       -D_FORTIFY_SOURCE=2
	       #-D_POSIX_C_SOURCE=200112L

LDFLAGS     += $(PKG_LDFLAGS) -pthread -ldl -Wl,--as-needed

# this makes clang shut up
COMPILER=$(shell $(CC) -v 2>&1 | grep version | awk '{print $$1}')
ifeq ($(COMPILER),gcc)
  WARN   += $(GCC_WARN)
  CFLAGS += $(GCC_CFLAGS)
endif

# Targets
all: $(CHECKVERS)

$(CHECKVERS): $(COMMON_OBJ) $(CHECKVERS_OBJ)
	@echo "[CCLD]	$@"
	@$(CC) $^ $(LDFLAGS) -o $@

src/checkvers/%.o: src/checkvers/%.c
	@echo "[CC]	$<"
	@$(CC) -I$(INC) $(CFLAGS) $(WARN) $(DEFS) -c $< -o $@

src/common/%.o: src/common/%.c
	@echo "[CC]	$<"
	@$(CC) -I$(INC) $(CFLAGS) $(WARN) $(DEFS) -c $< -o $@

%.d: %.c
	@echo "[CPP]	$@"
	@$(CPP) -MM -MG -nostdinc -I$(INC) -c $< -o $@

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
   -include $(DEPS)
endif

clean:
	$(RM) $(DEPS) $(CHECKVERS) $(CHECKVERS_OBJ) $(COMMON_OBJ)

.PHONY: all clean
