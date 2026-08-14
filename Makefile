CC = cc
AR = ar
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror -O2
CPPFLAGS = -Iinclude
ARFLAGS = rcs
PREFIX ?= /usr/local

LIB = libce.a

SRC = \
    src/core.c \
    src/mem.c \
    src/buf.c \
    src/str.c \
    src/file.c

OBJ = $(SRC:src/%.c=build/%.o)

.PHONY: all clean test install

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

build/%.o: src/%.c | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

build/test: tests/test.c $(LIB)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB) -o $@

test: build/test
	./build/test

install: $(LIB)
	install -d $(DESTDIR)$(PREFIX)/include/ce
	install -d $(DESTDIR)$(PREFIX)/lib
	install -m 644 include/ce/*.h $(DESTDIR)$(PREFIX)/include/ce/
	install -m 644 $(LIB) $(DESTDIR)$(PREFIX)/lib/

clean:
	rm -rf build $(LIB)
