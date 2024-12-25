# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g

# Target executable
TARGET = main

# Directories
SRC_DIR = ../src
SCANNER_DIR = $(SRC_DIR)/Scanner

# Source files
SRCS = $(SRC_DIR)/Main.cpp \
       $(SCANNER_DIR)/Scanner.cpp \
       $(SCANNER_DIR)/Token.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Include directories
INCLUDES = -I$(SRC_DIR) -I$(SCANNER_DIR)

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

# Phony targets
.PHONY: all clean
