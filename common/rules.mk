VALA      := $(wildcard *.vala)
SRC       := $(wildcard *.c)
OBJ       := $(SRC:%.c=%.o) 

all: $(NAME).a

$(NAME).a: $(OBJ)
	ar crs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install:

uninstall:

clean:
	$(RM) $(OBJ) $(NAME).a

distclean: clean
	$(RM) $(VALA:%.vala=%.c) $(NAME).h $(NAME).vapi Makefile

.PHONY: all install uninstall clean distclean
