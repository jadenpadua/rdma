#ifndef RDMA_SIM_H
#define RDMA_SIM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IBV_ACCESS_LOCAL_WRITE 0x1
#define IBV_ACCESS_REMOTE_WRITE 0x2
#define IBV_ACCESS_REMOTE_READ 0x4
#define IBV_QPT_RC 2

struct sim_device {
    char name[64];
    int device_id;
};

struct sim_context {
    struct sim_device *device;
    int is_open;
};

struct sim_pd {
    struct sim_context *context;
    int pd_id;
};

struct sim_mr {
    void *addr;
    size_t length;
    int lkey;
    int rkey;
    struct sim_pd *pd;
};

struct sim_cq {
    struct sim_context *context;
    int cq_size;
    int cq_id;
};

struct sim_qp {
    struct sim_pd *pd;
    struct sim_cq *send_cq;
    struct sim_cqe *recv_cq;
    int qp_num;
    char state[32];
};

struct qp_capabilities {
    int max_send_wr;
    int max_recv_wr;
    int max_send_sqe;
    int max_recv_sqe;
};

struct ibv_qp_init_attr {
    struct sim_cq *send_cq;
    struct sim_cq *recv_cq;
    int qp_type;
    struct qp_capabilities cap;
};

struct rdma_context {
    struct sim_context *context;
    struct sim_pd *pd;
    struct sim_mr *mr;
    struct sim_cq *cq;
    struct sim_qp *qp;
    char *buffer;
};

#endif // RDMA_SIM_H