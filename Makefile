# target name
TARGET = hid-core

# build path
BUILD_DIR = ./build

CC  = gcc
SZ  = size
DP  = objdump
OPT = -O0

C_FLAGS  = $(OPT) -g -Wall -Wextra -fdata-sections -ffunction-sections -static --coverage
LD_FLAGS = -T"gcc.ld" \
	   -lgcov -fprofile-arcs \

C_SOURCES = $(shell find ./app -name *.c) \
	    $(shell find ./src -name *.c) \

H_SOURCES  = \
-Iapp \
-Isrc \

OBJECT = $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

all: $(BUILD_DIR)/$(TARGET)

log:
	@echo 'c file: $(C_SOURCES)'
	@echo 'h file: $(H_SOURCES)'
	@echo $(OBJECT)

$(BUILD_DIR)/%.o:%.c Makefile | $(BUILD_DIR)
	@echo 'Build target o: $<'
	$(CC) $(C_FLAGS) -c $< $(H_SOURCES) -o $@

$(BUILD_DIR)/$(TARGET): $(OBJECT) Makefile
	$(CC) $(LD_FLAGS) $(OBJECT) -Wl,-Map,$(BUILD_DIR)/$(TARGET).map -o $@
	$(SZ) $<
	@echo "Build target:  $<.dump"
	@$(DP) -h -S $(BUILD_DIR)/$(TARGET) > $@.dump

$(BUILD_DIR):
	@mkdir $@

coverage:
	@gcovr -r ./

html:
	@gcovr -r ./ --html --html-details -o $(BUILD_DIR)/coverage/
	@firefox $(BUILD_DIR)/coverage/coverage_details.html

run: $(BUILD_DIR)/$(TARGET)
	@echo '$<:'
	@./$<

.PHONY: clean all dump log run

clean: $(BUILD_DIR)
	cd $(BUILD_DIR); rm -r coverage rm -f $(TARGET) *.gcno *.gcov *.gcda *.bin *.d *.o *.hex *.hexs *.erom *.list *.elf *.map *.rcf *.dump
