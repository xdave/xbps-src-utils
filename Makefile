NAME        := xbps-repo-checkvers

SRC         := $(wildcard src/*.c)
OBJ         := $(SRC:%.c=%.o)
DEP         := $(SRC:%.c=%.d)

STATIC      +=
PKG_STATIC  +=

PKGS        += glib-2.0 libxbps

PKG_CFLAGS  += $(shell pkg-config --cflags $(PKGS))
PKG_LDFLAGS += $(shell pkg-config --libs $(PKG_STATIC) $(PKGS))

NODEPS      := clean

GCC_WARN    += -Wunsafe-loop-optimizations \
	       -pedantic-errors
WARN        += -Wall \
	       -Wextra \
	       -Werror \
	       -Wshadow \
	       -Wformat=2 \
	       -Wmissing-prototypes \
	       -Wmissing-declarations \
	       -Wnested-externs \
	       -Wvla \
	       -Wno-overlength-strings \
	       -Wundef \
	       -Wsign-compare \
	       -Wmissing-include-dirs \
	       -Wold-style-definition \
	       -Winit-self \
	       -Wredundant-decls \
	       -Wfloat-equal \
	       -Wcast-align \
	       -Wcast-qual \
	       -Wpointer-arith \
	       -Wcomment \
	       -Wdeclaration-after-statement \
	       -Wwrite-strings \
	       -Wstack-protector

GCC_CFLAGS  += --param ssp-buffer-size=4
CFLAGS      += -ansi -O2 -pipe -mtune=generic -fstack-protector -fPIC \
	       $(PKG_CFLAGS)

DEFS        += -D_POSIX_C_SOURCE=200112L \
	       -D_FORTIFY_SOURCE=2

LDFLAGS     += $(PKG_LDFLAGS) -Wl,--as-needed

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
	@$(CC) $(STATIC) $^ $(LDFLAGS) -o $@

%.o: %.c
	@echo "[CC]	$<"
	@$(CC) -Iinclude -MMD $(CFLAGS) $(WARN) $(DEFS) -c $< -o $@

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
   -include $(DEP)
endif

clean:
	$(RM) $(NAME) $(OBJ) $(DEP)

.PHONY: all clean
