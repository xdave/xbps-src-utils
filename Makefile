NAME	:= repo-checkvers

CFLAGS	?= -ansi -O2 -pipe -mtune=generic -fPIC -fPIE \
	   -fstack-protector --param ssp-buffer-size=4 -Wall -Werror
DEFS	?= -D_POSIX_C_SOURCE=200112L -D_FORTIFY_SOURCE=2
LDFLAGS	?= -Wl,--as-needed -pie -Wl,-z,relro -Wl,-z,now

SRC	:= $(wildcard *.c)
OBJ	:= $(patsubst %.c,%.o,$(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

%.o: %.c
	$(CC) -c $< $(CFLAGS) $(DEFS) -o $@

clean:
	$(RM) $(NAME) $(OBJ)

.PHONY: all clean
