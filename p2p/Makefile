CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -libverbs

all: rdma_server

rdma_server: rdma_server.c
	$(CC) $(CFLAGS) -o rdma_server rdma_server.c $(LIBS)

clean:
	rm -f rdma_server


install_deps:
	@echo "Installing dependencies..."
	sudo apt-get install libibverbs-dev librdmacm-dev -y || \
	(sudo apt-get update && sudo apt-get install libibverbs-dev librdmacm-dev -y)

.PHONY: all clean install_deps