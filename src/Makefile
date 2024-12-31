# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g

# Target executable
TARGET = main

# Directories
SRC_DIR = ../src
SCANNER_DIR = $(SRC_DIR)/Scanner
PARSER_DIR = $(SRC_DIR)/Parser
INTERPRETER_DIR = $(SRC_DIR)/Interpreter
GENERATOR_DIR = $(SRC_DIR)/Generator

# Source files
SRCS = $(SRC_DIR)/Main.cpp \
       $(SCANNER_DIR)/Scanner.cpp \
       $(SCANNER_DIR)/Token.cpp \
       $(PARSER_DIR)/Parser.cpp \
       $(PARSER_DIR)/Printer.cpp \
	   $(INTERPRETER_DIR)/Interpreter.cpp \
	   $(INTERPRETER_DIR)/Datatype.cpp \
	   $(INTERPRETER_DIR)/BuiltinFunctionTable.cpp \
	   $(GENERATOR_DIR)/Generator.cpp \
	   $(GENERATOR_DIR)/Code.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Include directories
INCLUDES = -I$(SRC_DIR) -I$(SCANNER_DIR) -I$(PARSER_DIR) -I$(INTERPRETER_DIR) -I$(GENERATOR_DIR)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Debug using LLDB
debug: $(TARGET)
	lldb ./$(TARGET)

# Phony targets
.PHONY: all clean debug
