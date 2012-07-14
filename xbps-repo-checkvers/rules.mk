VALA    := $(wildcard *.vala)
SRC     := $(wildcard *.c)
OBJ     := $(SRC:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJ) ../common/common.a
	$(CC) $^ $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: $(NAME)
	mkdir -p $(PREFIX)/bin
	install -m755 $(NAME) $(PREFIX)/bin/$(NAME)

uninstall:
	test -f $(PREFIX)/bin/$(NAME) && rm -f $(PREFIX)/bin/$(NAME)

clean:
	$(RM) $(OBJ) $(NAME)

distclean: clean
	$(RM) $(VALA:%.vala=%.c) $(NAME).h $(NAME).vapi Makefile

.PHONY: all install clean distclean
