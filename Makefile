CC = cc
AR = ar
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Werror
CPPFLAGS = -Iinclude
LDFLAGS =

PREFIX ?= /usr/local
DESTDIR ?=
BUILD = build
LIB = $(BUILD)/libce.a
TEST = $(BUILD)/test
EXAMPLE = $(BUILD)/hello
PC = $(BUILD)/libce.pc
STAGE = $(BUILD)/stage
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=$(BUILD)/%.o)
SANITIZE ?=

ifneq ($(SANITIZE),)
CFLAGS += -fsanitize=$(SANITIZE) -fno-omit-frame-pointer
LDFLAGS += -fsanitize=$(SANITIZE)
endif

.PHONY: all clean test check examples install install-check format format-check

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

examples: $(EXAMPLE)

$(EXAMPLE): examples/hello.c $(LIB) | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $< $(LIB) -o $@

$(PC): libce.pc.in | $(BUILD)
	sed 's|@PREFIX@|$(PREFIX)|g' $< > $@

check: format-check clean all test examples install-check

install-check: $(LIB) $(PC)
	rm -rf $(STAGE)
	$(MAKE) install PREFIX=/usr DESTDIR=$(abspath $(STAGE))
	$(CC) -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Werror \
		-I$(STAGE)/usr/include examples/hello.c -L$(STAGE)/usr/lib -lce -o $(BUILD)/hello-installed
	$(BUILD)/hello-installed

format:
	clang-format -i $(SRC) $(wildcard include/ce/*.h) tests/test.c examples/*.c

format-check:
	clang-format --dry-run --Werror $(SRC) $(wildcard include/ce/*.h) tests/test.c examples/*.c

install: $(LIB) $(PC)
	install -d $(DESTDIR)$(PREFIX)/include/ce $(DESTDIR)$(PREFIX)/lib $(DESTDIR)$(PREFIX)/lib/pkgconfig
	install -m 644 include/ce/*.h $(DESTDIR)$(PREFIX)/include/ce/
	install -m 644 $(LIB) $(DESTDIR)$(PREFIX)/lib/libce.a
	install -m 644 $(PC) $(DESTDIR)$(PREFIX)/lib/pkgconfig/libce.pc

clean:
	rm -rf $(BUILD)

-include $(OBJ:.o=.d)
