# Set to @ to keep this makefile quiet
SILENCE = @

######################
### Compiler flags ###
######################
CFLAGS += -Wall
ifeq ($(FATAL_COMPILER_ERRORS),Y)
	CFLAGS += -Wfatal-errors
	CFLAGS += -Wextra
endif

INCLUDE_FLAGS = $(addprefix -I, $(INC_DIRS))



#############################
### Product Configuration ###
#############################
TARGET_NAME = $(PRODUCT)



#############################
### Product Configuration ###
#############################
# Include directory structure for a specific build configuration
include ProductConfig_$(PRODUCT).make

C_COMPILER = gcc
DEP_FLAGS = -MMD -MP
MAKE_DIR = mkdir -p
REMOVE = rm -rf



#######################
### Auto-generation ###
#######################
# Auto-generate list of source code
SRC = $(call get_c_src_from_dir_list, $(SRC_DIRS))
src_obj = $(call c_to_o, $(SRC))
SRC_OBJ = $(addprefix $(OBJECT_DIR)/, $(src_obj))
src_dep = $(call c_to_d, $(SRC))
SRC_DEP = $(addprefix $(OBJECT_DIR)/, $(src_dep))

DEP_FILES = $(SRC_DEP)

TARGET = $(BUILD_DIR)/$(TARGET_NAME)



########################
### Helper functions ###
########################
get_c_src_from_dir_list = $(foreach dir, $1, $(call get_c_src_from_dir, $(dir)))
get_c_src_from_dir = $(wildcard $1/*.c)
c_to_o = $(call convert_extension,.c,.o,$1)
c_to_d = $(call convert_extension,.c,.d,$1)

get_cpp_src_from_dir_list = $(foreach dir, $1, $(call get_cpp_src_from_dir, $(dir)))
get_cpp_src_from_dir = $(wildcard $1/*.cpp)
cpp_to_o = $(call convert_extension,.cpp,.o,$1)
cpp_to_d = $(call convert_extension,.cpp,.d,$1)

# $1 is the initial extension
# $2 is the final extension
# $3 is the file in question
convert_extension = $(patsubst %$1,%$2,$3)



ifneq ("(MAKECMDGOALS)","clean")
-include $(DEP_FILES)
endif



###########
# Targets #
###########
.PHONY: all clean full_clean screen_clear

all: $(TARGET)
	@echo Compiled: $(TARGET)

$(TARGET): $(SRC_OBJ)
	$(SILENCE)$(QUIET)$(MAKE_DIR) $(dir $@)
	$(SILENCE)$(C_COMPILER) -o $@ $^ $(INCLUDE_FLAGS) $(CFLAGS) $(CXXFLAGS) $(LD_LIBRARIES)

# Compile source .c files
$(OBJECT_DIR)/%.o: %.c
	$(SILENCE)$(QUIET)$(MAKE_DIR) $(dir $@)
	$(SILENCE)$(C_COMPILER) $(DEP_FLAGS) -o $@ -c $< $(CFLAGS) $(INCLUDE_FLAGS)

filelist:
	@echo $(SRC_OBJ)
	@echo $(SRC_DEP)

clean:
	$(SILENCE)$(QUIET)$(REMOVE) $(TARGET)
	$(SILENCE)$(QUIET)$(REMOVE) $(SRC_OBJ)
	$(SILENCE)$(QUIET)$(REMOVE) $(SRC_DEP)

full_clean:
	$(SILENCE)$(QUIET)$(REMOVE) $(BUILD_DIR)
	$(SILENCE)$(QUIET)$(REMOVE) $(OBJECT_DIR)

screen_clear:
	@clear
