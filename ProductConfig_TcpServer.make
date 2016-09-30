#############################
### Project configuration ###
#############################
#ROOT_DIRECTORY=
SRC_DIRS = src src_platform products/$(PRODUCT)
INC_DIRS = inc src_platform products/$(PRODUCT)
BUILD_DIR = build
OBJECT_DIR = obj
LIBRARIES = pthread

# Pass project configuration to the submake
export SRC_DIRS
export INC_DIRS
export BUILD_DIR
export OBJECT_DIR
