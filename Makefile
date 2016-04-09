
TARGET  = hid_output_test_ds4

CC := gcc
CFLAGS := -g -MMD -MP -Wall -Wextra
LDFLAGS = -lz
LIBS    =
SRC_DIR = ./src
OBJ_DIR = .

SOURCES = $(shell ls $(SRC_DIR)/*.c)
OBJS    = $(subst $(SRC_DIR),$(OBJ_DIR), $(SOURCES:.c=.o))
DEPENDS = $(OBJS:.o=.d)

DEPENDS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

clean:
	$(RM) $(OBJS) $(TARGET) $(DEPENDS)

-include $(DEPENDS)

.PHONY: all clean
