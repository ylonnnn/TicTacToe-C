CC = gcc 
CCFLAGS = -g -Wall -Wextra -std=c11

BUILD_DIR = $(CURDIR)/build

SOURCES = $(subst \,/,$(shell for /R %%f in (*.c) do @echo %%f))
OBJECTS = $(patsubst %.c, %.o,$(subst $(CURDIR),$(BUILD_DIR),$(SOURCES)))
TARGET = $(BUILD_DIR)/compiled.exe

$(BUILD_DIR)/%.o: %.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CC) $(CCFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

crun: $(TARGET)
	$(TARGET)

