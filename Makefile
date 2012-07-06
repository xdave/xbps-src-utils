NAME		:= xbps-repo-checkvers

PKG_STATIC	:= # --static
STATIC		:= # -static

PKGS		:= glib-2.0 libxbps

PKG_CFLAGS	:= $(shell pkg-config --cflags $(PKGS))
PKG_LDFLAGS	:= $(shell pkg-config --libs $(PKG_STATIC) $(PKGS))

WARN		+= -Wall -Wextra -Werror -Wshadow -Wformat=2 \
		-Wmissing-prototypes -Wmissing-declarations -Wnested-externs \
		-Wvla -Wno-overlength-strings -Wunsafe-loop-optimizations \
		-Wundef -Wsign-compare -Wmissing-include-dirs \
		-Wold-style-definition -Winit-self -Wredundant-decls \
		-Wfloat-equal -Wcast-align -Wcast-qual -Wpointer-arith \
		-Wcomment -Wdeclaration-after-statement -Wwrite-strings \
		-Wstack-protector

CFLAGS		+= -O2 -pipe -mtune=generic -fPIC -ansi $(WARN) $(PKG_CFLAGS) \
		   -fstack-protector --param ssp-buffer-size=4

DEFS		+= -D_POSIX_C_SOURCE=200112L -D_FORTIFY_SOURCE=2
LDFLAGS		+= -Wl,--as-needed $(PKG_LDFLAGS)

SRC		:= $(wildcard src/*.c)
OBJ		:= $(patsubst %.c,%.o,$(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(STATIC) $^ $(LDFLAGS) -o $@

%.o: %.c
	$(CC) -c $< $(CFLAGS) -Iinclude $(DEFS) -o $@

clean:
	$(RM) $(NAME) $(OBJ)

.PHONY: all clean
