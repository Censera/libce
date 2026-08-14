CC = cc
AR = ar
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Werror
CPPFLAGS = -Iinclude
LDFLAGS =

PREFIX ?= /usr/local
BUILD = build
LIB = $(BUILD)/libce.a
TEST = $(BUILD)/test
PC = $(BUILD)/libce.pc
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=$(BUILD)/%.o)
SANITIZE ?=

ifneq ($(SANITIZE),)
CFLAGS += -fsanitize=$(SANITIZE) -fno-omit-frame-pointer
LDFLAGS += -fsanitize=$(SANITIZE)
endif

.PHONY: all clean test check install format format-check

all: $(LIB)

$(BUILD):
	mkdir -p $@

$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(LIB): $(OBJ)
	$(AR) rcs $@ $^

test: $(TEST)
	./$(TEST)

$(TEST): tests/test.c $(LIB) | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $< $(LIB) -o $@

$(PC): libce.pc.in | $(BUILD)
	sed 's|@PREFIX@|$(PREFIX)|g' $< > $@

check: format-check clean all test

format:
	clang-format -i $(SRC) $(wildcard include/ce/*.h) tests/test.c

format-check:
	clang-format --dry-run --Werror $(SRC) $(wildcard include/ce/*.h) tests/test.c

install: $(LIB) $(PC)
	install -d $(PREFIX)/include/ce $(PREFIX)/lib $(PREFIX)/lib/pkgconfig
	install -m 644 include/ce/*.h $(PREFIX)/include/ce/
	install -m 644 $(LIB) $(PREFIX)/lib/libce.a
	install -m 644 $(PC) $(PREFIX)/lib/pkgconfig/libce.pc

clean:
	rm -rf $(BUILD)

-include $(OBJ:.o=.d)
