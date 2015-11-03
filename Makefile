CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -pedantic -I. -L.
AR=ar
AFLAGS=-cvq

TARGET=libslex.a
SOURCES=slex.c
OBJECTS=slex.o

TEST_TARGET=slex_test
TEST_SOURCES=slex_test.c

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) $(AFLAGS) $@ $^

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ -c $^

test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_SOURCES) $(TARGET)
	$(CC) $(CFLAGS) -o $@ $< -lslex

clean:
	rm -f $(TARGET) $(OBJECTS) $(TEST_TARGET)

.PHONY: clean
