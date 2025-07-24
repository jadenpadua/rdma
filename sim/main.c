#include <unistd.h>
#include "rdma_sim.h"

int main() {
    struct rdma_context ctx = {0};

    if(setup_rdma_context(&ctx) < 0) {
        fprintf(stderr, "Failed to set up RDMA context\n");
        return -1;
    }
    return 0;
}
