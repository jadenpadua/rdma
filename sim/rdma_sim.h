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
// device related
struct sim_device **ibv_get_device_list(int *num_devices);
void ibv_free_device_list(struct sim_device **list);
struct sim_context *ibv_open_device(struct sim_device *device);
int ibv_close_device(struct sim_context *context);
// protection domain related
struct sim_pd *ibv_alloc_pd(struct sim_context *context);
int ibv_dealloc_pd(struct sim_pd *pd);
struct sim_mr *ibv_reg_mr(struct sim_pd *pd, void* addr, size_t length, int access);
int ibv_dereg_mr(struct sim_mr *mr);
// completion queue related
struct sim_cq *ibv_create_cq(struct sim_context *context, int cqe, void *cq_context, void *channel, int comp_vector);
int ibv_destroy_cq(struct sim_cq *cq);
// qp related
struct sim_cqp *ibv_create_qp(struct sim_pd *pd, struct ibv_qp_init_attr *attr);
int ibv_destroy_qp(struct sim_qp *qp);
// helper functions
int setup_rdma_context(struct rdma_context *ctx);
void cleanup_rdma_context(struct rdma_context *ctx);
void simulate_rdma_operations(struct rdma_context *ctx);

#endif // RDMA_SIM_H