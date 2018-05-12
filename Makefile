SRC_DIR := src
BUILD_DIR := build
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
BUILD_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
CFLAGS := -Wall -Wextra -static -g -O3
CC := gcc


uctps: $(BUILD_FILES) uctps.c
	$(CC) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)

zip:
	zip xmarus06.zip src/* uctps.c doc.pdf tests/* Makefile
