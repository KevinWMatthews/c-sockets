#############################
### Project configuration ###
#############################
#ROOT_DIRECTORY=
SRC_DIRS = src src_platform products/$(PRODUCT)
INC_DIRS = inc src_platform products/$(PRODUCT)
TEST_DIRS = tests/$(TEST_MODULE)
MOCK_DIRS = mocks/$(TEST_MODULE)
BUILD_DIR = build
OBJECT_DIR = obj

# Pass project configuration to the submake
export SRC_DIRS
export INC_DIRS
export TEST_DIRS
export MOCK_DIRS
export BUILD_DIR
export OBJECT_DIR
