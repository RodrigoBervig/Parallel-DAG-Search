# Compiler and flags
CXX := g++
BASE_FLAGS := -std=c++17
PAR_FLAGS := -fopenmp

# Source files
SEQ_SRC := sequential_implementation.cpp
PAR_SRC := parallel_implementation.cpp
GEN_SRC := random_dag_generator.cpp
GRAPH_SRC := graph.c

# Output directories
BIN_DIR := bin
DEBUG_DIR := $(BIN_DIR)/debug
OPT_DIR := $(BIN_DIR)/optimized
PERF_DIR := $(BIN_DIR)/perf

# Default target
.PHONY: all
all: debug

# Ensure directories exist
dirs:
	mkdir -p $(DEBUG_DIR) $(OPT_DIR) $(PERF_DIR)

# Build types
.PHONY: debug optimized perf clean

debug: CFLAGS := -g -Wall -fsanitize=address
debug: dirs \
	$(DEBUG_DIR)/sequential \
	$(DEBUG_DIR)/parallel \
	$(DEBUG_DIR)/generator \
	$(DEBUG_DIR)/graph

optimized: CFLAGS := -O3
optimized: dirs \
	$(OPT_DIR)/sequential \
	$(OPT_DIR)/parallel \
	$(OPT_DIR)/generator \
	$(OPT_DIR)/graph

perf: CFLAGS := -O2 -pg
perf: dirs \
	$(PERF_DIR)/sequential \
	$(PERF_DIR)/parallel \
	$(PERF_DIR)/generator \
	$(PERF_DIR)/graph

# Build rules
$(DEBUG_DIR)/sequential: $(SEQ_SRC)
	$(CXX) $(BASE_FLAGS) $(CFLAGS) -o $@ $^

$(DEBUG_DIR)/parallel: $(PAR_SRC)
	$(CXX) $(BASE_FLAGS) $(CFLAGS) $(PAR_FLAGS) -o $@ $^

$(DEBUG_DIR)/generator: $(GEN_SRC)
	$(CXX) $(BASE_FLAGS) $(CFLAGS) -o $@ $^

$(DEBUG_DIR)/graph: $(GRAPH_SRC)
	$(CXX) $(BASE_FLAGS) $(CFLAGS) -o $@ $^

$(OPT_DIR)/sequential: $(SEQ_SRC)
	$(CXX) $(BASE_FLAGS) $(CFLAGS) -o $@ $^

$(OPT_DIR)/parallel: $(PAR_SRC)
	$(CXX) $(BASE_FLAGS) $(CFLAGS) $(PAR_FLAGS) -o $@ $^

$(OPT_DIR)/generator: $(GEN_SRC)
	$(CXX) $(BASE_FLAGS) $(CFLAGS) -o $@ $^

$(OPT_DIR)/graph: $(GRAPH_SRC)
	$(CXX) $(BASE_FLAGS) $(CFLAGS) -o $@ $^

$(PERF_DIR)/sequential: $(SEQ_SRC)
	$(CXX) $(BASE_FLAGS) $(CFLAGS) -o $@ $^

$(PERF_DIR)/parallel: $(PAR_SRC)
	$(CXX) $(BASE_FLAGS) $(CFLAGS) $(PAR_FLAGS) -o $@ $^

$(PERF_DIR)/generator: $(GEN_SRC)
	$(CXX) $(BASE_FLAGS) $(CFLAGS) -o $@ $^

$(PERF_DIR)/graph: $(GRAPH_SRC)
	$(CXX) $(BASE_FLAGS) $(CFLAGS) -o $@ $^

# Clean rule
clean:
	rm -rf $(BIN_DIR)
