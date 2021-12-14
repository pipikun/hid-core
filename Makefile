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
	   -fprofile-arcs \
#	   -Wl,--verbose

C_SOURCES = $(shell find ./app -name *.c) \
	    $(shell find ./src -name *.c) \

H_SOURCES  = \
-Iapp \
-Isrc \

OBJECT = $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

all: $(BUILD_DIR)/$(TARGET) gen_json

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

gen_json:
	@gcovr -r . --json-pretty -o $(BUILD_DIR)/$(TARGET).json

regen_json:
	@gcovr -a $(BUILD_DIR)/$(TARGET).json --json-pretty -o $(BUILD_DIR)/new-$(TARGET).json

coverage:
	gcovr -a $(BUILD_DIR)/$(TARGET).json

html:
	@gcovr -a $(BUILD_DIR)/$(TARGET).json --html --html-details -o $(BUILD_DIR)/$(TARGET).html
	@firefox $(BUILD_DIR)/$(TARGET).html

run: $(BUILD_DIR)/$(TARGET)
	@echo '$<:'
	@./$<

.PHONY: clean all dump log run

clean: $(BUILD_DIR)
	@cd $(BUILD_DIR); rm -r html rm -f $(TARGET) *.gcno *.gcov\
	       			*.gcda *.bin *.d *.o *.hex *.hexs *.erom \
				*.list *.elf *.map *.rcf *.dump *.json
