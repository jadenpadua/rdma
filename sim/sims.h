#ifndef SIMS_H
#define SIMS_H

#include "rdma.h"

/**
 * @brief Simulates RDMA operations including QP state transitions and data
 * transfers
 *
 * This function demonstrates the typical RDMA workflow:
 * - Transitions Queue Pair through required states (RESET -> INIT -> RTR ->
 * RTS)
 * - Performs RDMA READ operations
 * - Performs RDMA WRITE operations
 * - Polls completion queue for completed operations
 *
 * @param ctx Pointer to the RDMA context containing initialized RDMA resources
 */
void simulate_rdma_operations(struct rdma_context *ctx);

#endif // SIMS_H