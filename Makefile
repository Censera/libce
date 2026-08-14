CC = cc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Werror
CPPFLAGS = -Iinclude

BUILD = build
LIB = $(BUILD)/libce.a
TEST = $(BUILD)/test_ce

SRC = src/ce.c
OBJ = $(SRC:src/%.c=$(BUILD)/%.o)

.PHONY: all clean test

all: $(LIB)

$(BUILD):
	mkdir -p $@

$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJ)
	ar rcs $@ $^

test: $(TEST)
	./$(TEST)

$(TEST): tests/test_ce.c $(LIB) | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB) -o $@

clean:
	rm -rf $(BUILD)
