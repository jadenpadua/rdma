CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SOURCES = main.c rdma_sim.c
HEADERS = rdma_sim.h
SIMULATOR = rdma_simulator

all: simulator

# Simulator - no RDMA libraries needed
simulator: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(SIMULATOR) $(SOURCES)

run: simulator
	./$(SIMULATOR)

clean:
	rm -f $(SIMULATOR)

test: simulator
	@echo "Testing simulator..."
	./$(SIMULATOR)
	@echo "Simulator test complete"


files:
	@echo "Project structure:"
	@echo "  rdma_sim.h     - Header with structs and function declarations"
	@echo "  rdma_sim.c     - Implementation of RDMA simulation functions"
	@echo "  main.c         - Clean main function"
	@echo "  Makefile       - Build configuration"

.PHONY: all clean simulator run test files