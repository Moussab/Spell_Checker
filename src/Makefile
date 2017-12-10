CC      = gcc

CFLAGS  = -g -std=c99 -Wall -Wextra -Werror -pedantic

EXE  = spell

SRCS = spell.c dictionary.c my_strmap.c rb_tree.c
OBJS = ${SRCS:.c=.o}
DEPS = ${SRCS:.c=.d}
HDRS = dictionary.h

BINDIR = ../bin

all: $(EXE)
	mv $(EXE) $(BINDIR)
	make clean

$(EXE): $(OBJS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

-include ${DEPS}

clean:
	rm -f ${OBJS} ${DEPS}

