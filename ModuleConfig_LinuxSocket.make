#############################
### Project configuration ###
#############################
#ROOT_DIRECTORY=
SRC_DIRS = src/Linux
INC_DIRS = inc inc/Linux
TEST_DIRS = tests/$(TEST_MODULE)
MOCK_DIRS = mocks/LinuxSocket
BUILD_DIR = build
OBJECT_DIR = obj

# Pass project configuration to the submake
export SRC_DIRS
export INC_DIRS
export TEST_DIRS
export MOCK_DIRS
export BUILD_DIR
export OBJECT_DIR
