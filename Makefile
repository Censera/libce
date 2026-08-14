CC = cc
AR = ar
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Werror
CPPFLAGS = -Iinclude

PREFIX ?= /usr/local

BUILD = build
LIB = $(BUILD)/libce.a
TEST = $(BUILD)/test_ce

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=$(BUILD)/%.o)

.PHONY: all clean test install

all: $(LIB)

$(BUILD):
	mkdir -p $@

$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJ)
	$(AR) rcs $@ $^

test: $(TEST)
	./$(TEST)

$(TEST): tests/test.c $(LIB) | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB) -o $@

install: $(LIB)
	install -d $(PREFIX)/include/ce $(PREFIX)/lib
	install -m 644 include/ce/*.h $(PREFIX)/include/ce/
	install -m 644 $(LIB) $(PREFIX)/lib/libce.a

clean:
	rm -rf $(BUILD)
