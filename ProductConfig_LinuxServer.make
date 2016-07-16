#############################
### Project configuration ###
#############################
#ROOT_DIRECTORY=
PLATFORM = Linux
SRC_DIRS = src src_platform/$(PLATFORM) products/$(PRODUCT)
INC_DIRS = inc
BUILD_DIR = build
OBJECT_DIR = obj
LIBRARIES = pthread

# Pass project configuration to the submake
export SRC_DIRS
export INC_DIRS
export BUILD_DIR
export OBJECT_DIR
export LIBRARIES
