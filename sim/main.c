#include <unistd.h>
#include "rdma_sim.h"

int main() {
    struct rdma_context ctx = {0};

    if(setup_rdma_context(&ctx) < 0) {
        fprintf(stderr, "Failed to set up RDMA context\n");
        return -1;
    }
    // put a message in the buffer
    strcpy(ctx.buffer, "Hello RDMA World!");
    printf("Message in buffer: %s\n", ctx.buffer);
    printf("Buffer address: %p\n", (void*)ctx.buffer);
    printf("Remote key for access to buffer: %d\n", ctx.mr->rkey);
    printf("This buffer is now accessible remotely!\n");

    simulate_rdma_operations(&ctx);

    cleanup_rdma_context(&ctx);
    return 0;
}
