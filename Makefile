NAME	:= repo-checkvers

CFLAGS	?= -ansi -O2 -pipe -mtune=generic -fPIC \
	   -fstack-protector --param ssp-buffer-size=4 \
	   -Wall -Werror -Wformat -Wformat-security -Werror=format-security
DEFS	?= -D_POSIX_C_SOURCE=200112L -D_FORTIFY_SOURCE=2
LDFLAGS	?= -Wl,--as-needed -Wl,-z,relro -Wl,-z,now

SRC	:= $(wildcard *.c)
OBJ	:= $(patsubst %.c,%.o,$(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -static $^ $(LDFLAGS) -o $@

%.o: %.c
	$(CC) -c $< $(CFLAGS) $(DEFS) -o $@

clean:
	$(RM) $(NAME) $(OBJ)

.PHONY: all clean
