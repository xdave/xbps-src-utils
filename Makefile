NAME		:= xbps-repo-checkvers

PKG_STATIC	:= # --static
STATIC		:= # -static

PKGS		:= glib-2.0 libxbps

GLIB_CFLAGS	:= $(shell pkg-config --cflags $(PKGS))
GLIB_LDFLAGS	:= $(shell pkg-config --libs $(PKG_STATIC) $(PKGS))

WARN		:= -Wall -Wextra -Werror -Wshadow -Wformat=2 \
		-Wmissing-prototypes -Wmissing-declarations -Wnested-externs \
		-Wvla -Wno-overlength-strings -Wunsafe-loop-optimizations \
		-Wundef -Wsign-compare -Wmissing-include-dirs \
		-Wold-style-definition -Winit-self -Wredundant-decls \
		-Wfloat-equal -Wcast-align -Wcast-qual -Wpointer-arith \
		-Wcomment -Wdeclaration-after-statement -Wwrite-strings \
		-Wstack-protector

CFLAGS		?= -ggdb -ansi -O3 -pipe -mtune=generic -fPIC \
		-fstack-protector --param ssp-buffer-size=4 \
		$(WARN) $(GLIB_CFLAGS)

DEFS		?= -D_POSIX_C_SOURCE=200112L -D_FORTIFY_SOURCE=2
LDFLAGS		?= -Wl,--as-needed -Wl,-z,relro -Wl,-z,now $(GLIB_LDFLAGS)

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
