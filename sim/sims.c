#include "sims.h"
#include <stdio.h>
#include <string.h>

void simulate_rdma_operations(struct rdma_context *ctx) {
  printf("\nSimulating RDMA operations...\n");
  printf("===============================================\n");

  printf("Transitioning QP through required states\n");
  printf("RESET -> INIT -> RTR -> RTS\n");
  strcpy(ctx->qp->state, "RTS");
  printf("QP transitioned to RTS state - ready for data transfer\n\n");

  printf("Performing RDMA READ operation:\n");
  printf("ibv_post_send(qp, RDMA_READ, remote_addr=0x%x)\n", ctx->mr->rkey);
  printf("Reading remote buffer contents: '%s'\n", ctx->buffer);
  printf("Direct memory access - bypassing remote CPU\n\n");

  printf("Performing RDMA WRITE operation:\n");
  printf("ibv_post_send(qp, RDMA_WRITE, remote_addr=0x%x)\n", ctx->mr->rkey);
  printf("Writing data directly to remote memory buffer\n");
  printf("Remote CPU remains idle during transfer\n\n");

  printf("Polling completion queue:\n");
  printf("ibv_poll_cq(cq) - checking for completed operations\n");
  printf("Work completion events processed successfully\n\n");

  printf("RDMA operations completed - zero-copy data transfer achieved\n");
}