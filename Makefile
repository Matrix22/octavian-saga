# COMPILER RULES
CC := g++
CFLAGS := -Wall -Wextra -Werror -std=c++17
LDFLAGS := -lm

# ARCHIVE RULES
ZIP := zip
ZIP_FLAGS := -FSr
ARCHIVE_NAME := 323CD_NegruMihai.zip
ARCHIVE_FILES := *.cpp *.hpp *.h Makefile README

# TARGET FILES RULES
SRC_FILES := $(wildcard *.cpp)
EXEC_FILES := $(patsubst %.cpp,%,$(SRC_FILES))
SAT_FILES := sat.cnf sat.sol

# REMOVING RULES
RM := rm
RM_FLAGS := -rf

.PHONY: pack build clean

# BUILDING EXECUTABLES FILES RULES
build: $(EXEC_FILES)

%: %.cpp
	@$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@


# RUNNING EXECUTABLES FILES RULES
run_%: %
	@./$<


# CLEANING RULES
clean:
	@$(RM) $(RM_FLAGS) $(EXEC_FILES) $(SAT_FILES)


# PACKING RULES
pack:
	@$(ZIP) $(ZIP_FLAGS) $(ARCHIVE_NAME) $(ARCHIVE_FILES)
