CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SOURCES = main.c rdma.c sims.c
HEADERS = rdma.h sims.h
SIMULATOR = rdma_sim

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
	@echo "  rdma.h     - Header with structs and function declarations"
	@echo "  sims.h     - Header for all simulation functions"
	@echo "  rdma.c     - Implementation of RDMA simulation functions"
	@echo "  sims.c     - Implementation of all simulation functions"
	@echo "  main.c         - Clean main function"
	@echo "  Makefile       - Build configuration"

.PHONY: all clean simulator run test files