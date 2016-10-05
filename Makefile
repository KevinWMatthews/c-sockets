# Set to @ to keep this makefile quiet
SILENCE = @

ALL_PRODUCTS =	\
				ServerDemo \
				ClientDemo \
				UdpListener \
				UdpClient \
				TcpServer

ALL_TEST_MODULES = \
					Socket



##############################
### Makefile configuration ###
##############################
# Set to 'Y' to cause a single compiler error to kill the make.
FATAL_COMPILER_ERRORS = Y
export FATAL_COMPILER_ERRORS

# Set to 'Y' to turn on debug capabilities
DEBUG=Y
export DEBUG

# Set to 'Y' to suppress makefile errors when running unit tests.
IGNORE_UNIT_TEST_ERRORS = N
ifeq ($(IGNORE_UNIT_TEST_ERRORS),Y)
	IGNORE_ERROR = -
endif
export IGNORE_ERROR

# Set to 'Y' to suppress makefile messages when entering and leaving sub-makes.
SUPPRESS_ENTERING_DIRECTORY_MESSAGE = Y
ifeq ($(SUPPRESS_ENTERING_DIRECTORY_MESSAGE),Y)
	NO_PRINT_DIRECTORY = --no-print-directory
endif

PRODUCTION_MAKEFILE = Makefile_Production.make
TEST_MAKEFILE = Makefile_CppUTest.make
MAKE = make $(NO_PRINT_DIRECTORY) --file
CLEAR = clear



#############################
### Auto-generated values ###
#############################
# If user did not specify a product to build, set one by default.
ifeq ($(strip $(PRODUCT)),)
	PRODUCTS = $(ALL_PRODUCTS)
endif

# If user did not specify a module to test, test all of them.
ifeq ($(strip $(TEST_MODULES)),)
	TEST_MODULES = $(ALL_TEST_MODULES)
endif



###############
### Targets ###
###############
# Generating the target for the submake in this fashion enables us to use the TEST_MODULES target.
ifeq ($(MAKECMDGOALS),)
	SUBMAKE_GOAL = all
endif
ifeq ($(MAKECMDGOALS),test)
	SUBMAKE_GOAL = test
endif
ifeq ($(MAKECMDGOALS),clean)
	SUBMAKE_GOAL = clean
endif
ifeq ($(MAKECMDGOALS),full_clean)
	SUBMAKE_GOAL = full_clean
endif

.PHONY: all linux test clean full_clean $(TEST_MODULES) screen_clear
linux: $(PRODUCTS)

test: screen_clear $(TEST_MODULES)
	@echo "Built and tested: $(TEST_MODULES)"

clean: $(PRODUCTS) $(TEST_MODULES)
	@echo "Cleaned: $(PRODUCTS)"
	@echo "Cleaned: $(TEST_MODULES)"

full_clean: $(TEST_MODULES)
	@echo "Fully cleaned: $(TEST_MODULES)"

$(PRODUCTS):
	$(SILENCE)$(MAKE) $(PRODUCTION_MAKEFILE) $(SUBMAKE_GOAL) PRODUCT=$@

$(TEST_MODULES):
# Using $@ trims the whitespace
	$(SILENCE)$(MAKE) $(TEST_MAKEFILE) $(SUBMAKE_GOAL) TEST_MODULE=$@

# clear the terminal screen
screen_clear:
	$(CLEAR)
