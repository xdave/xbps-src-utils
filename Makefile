NAME		:= repo-checkvers

PKG_STATIC	:= # --static
STATIC		:= # -static

GLIB_CFLAGS	:= $(shell pkg-config --cflags glib-2.0)
GLIB_LDFLAGS	:= $(shell pkg-config --libs $(PKG_STATIC) glib-2.0)

CFLAGS		?= -ansi -O3 -pipe -mtune=generic -fPIC \
	   		-fstack-protector --param ssp-buffer-size=4 \
	   		-Wall -Werror -Wformat -Wformat-security \
			$(GLIB_CFLAGS)
DEFS		?= -D_POSIX_C_SOURCE=200112L -D_FORTIFY_SOURCE=2
LDFLAGS		?= -Wl,--as-needed -Wl,-z,relro -Wl,-z,now \
		   	$(GLIB_LDFLAGS)

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
