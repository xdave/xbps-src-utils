NAME        := xbps-repo-checkvers

SRC         := $(wildcard src/*.c)
OBJ         := $(SRC:%.c=%.o)
DEP         := $(SRC:%.c=%.d)

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
	       -Wfatal-errors \
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
all: $(NAME)

$(NAME): $(OBJ)
	@echo "[CCLD]	$@"
	@$(CC) $^ $(LDFLAGS) -o $@

%.o: %.c
	@echo "[CC]	$<"
	@$(CC) -Iinclude -MMD $(CFLAGS) $(WARN) $(DEFS) -c $< -o $@

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
   -include $(DEP)
endif

clean:
	$(RM) $(NAME) $(OBJ) $(DEP)

.PHONY: all clean
