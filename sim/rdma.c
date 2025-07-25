#include "rdma.h"
#include "sims.h"

#define BUFFER_SIZE 1024
#define USE_GPU_DIRECT 1

// simulate device list (mock hardware)
static struct sim_device sim_devices[] = {{"mlx5_0", 0, USE_GPU_DIRECT},
                                          {"mlx5_1", 1, USE_GPU_DIRECT},
                                          {"mlx5_2", 2, USE_GPU_DIRECT}};

struct sim_device **ibv_get_device_list(int *num_devices) {
  static struct sim_device *device_ptrs[3];
  printf("ibv_get_device_list() - Discovering RDMA devices...\n");
  // simulate finding 2 devices
  device_ptrs[0] = &sim_devices[0];
  device_ptrs[1] = &sim_devices[1];
  device_ptrs[2] = NULL; // Null-terminate the list

  if (num_devices) {
    *num_devices = 2;
  }
  printf("Found %d RDMA devices \n", *num_devices);
  printf("Device 0: %s, ID: %d\n", device_ptrs[0]->name,
         device_ptrs[0]->device_id);
  printf("Device 1: %s, ID: %d\n", device_ptrs[1]->name,
         device_ptrs[1]->device_id);

  return device_ptrs;
}

struct sim_context *ibv_open_device(struct sim_device *device) {
  printf("ibv_open_device() - Opening device: %s\n", device->name);

  struct sim_context *ctx = malloc(sizeof(struct sim_context));
  ctx->device = device;
  ctx->is_open = 1;

  printf("Device opened successfully: %s\n", device->name);
  printf("Context created at %p\n", (void *)ctx);

  return ctx;
}

void ibv_free_device_list(struct sim_device **list) {
  printf("ibv_free_device_list() - Freeing device list at address %p\n",
         (void *)list);
  // In a real implementation, we would free each device structure
  // Here we just print a message as we are using static devices
}

struct sim_pd *ibv_alloc_pd(struct sim_context *context) {
  printf("ibv_alloc_pd() - Allocating protection domain...\n");

  struct sim_pd *pd = malloc(sizeof(struct sim_pd));
  pd->context = context;
  pd->pd_id = 12345;

  printf("Protection domain allocated with ID: %d\n", pd->pd_id);
  printf("This Protection domain will group all related RDMA resources\n");

  return pd;
}

struct sim_mr *ibv_reg_mr(struct sim_pd *pd, void *addr, size_t length,
                          int access) {
  printf("ibv_reg_mr() - Registering memory region...\n");
  printf("Address: %p, Length: %zu, Access: %d\n", addr, length, access);
  struct sim_mr *mr = malloc(sizeof(struct sim_mr));
  mr->gpu_addr = addr;
  mr->length = length;
  mr->lkey = 0xABCD1234; // Simulated local key
  mr->rkey = 0xEF567890; // Simulated remote key
  mr->pd = pd;

  printf("Memory region registered with LKey: 0x%X, RKey: 0x%X\n", mr->lkey,
         mr->rkey);
  printf("Remote clients can now access this memory region using the RKey\n");

  return mr;
}

struct sim_cq *ibv_create_cq(struct sim_context *context, int cqe,
                             void *cq_context, void *channel, int comp_vector) {
  printf("ibv_create_cq() - Creating Completion Queue...\n");
  struct sim_cq *cq = malloc(sizeof(struct sim_cq));
  cq->context = context;
  cq->cq_size = cqe;
  cq->cq_id = 9999;

  printf("Completion Queue created with ID: %d size: %d cq_context: %p "
         "channel: %p comp_vector: %d\n",
         cq->cq_id, cq->cq_size, cq_context, channel, comp_vector);
  printf("This Completion Queue will hold completion events for operations\n");
  return cq;
}

struct sim_qp *ibv_create_qp(struct sim_pd *pd,
                             struct ibv_qp_init_attr *qp_init_attr) {
  printf("ibv_create_qp() - Creating Queue Pair...\n");

  struct sim_qp *qp = malloc(sizeof(struct sim_qp));
  qp->pd = pd;
  qp->send_cq = qp_init_attr->send_cq;
  qp->recv_cq = qp_init_attr->recv_cq;
  qp->qp_num = 7777; // Simulated QP number
  strcpy(qp->state, "RESET");
  printf("Queue Pair created with QP number: %d\n", qp->qp_num);
  printf("Send CQ: %p, Recv CQ: %p\n", (void *)qp->send_cq,
         (void *)qp->recv_cq);
  printf("This QP is your RDMA socket for communication\n");

  return qp;
}

int ibv_destroy_qp(struct sim_qp *qp) {
  printf("ibv_destroy_qp() - Destroying Queue Pair with QP number: %d\n",
         qp->qp_num);
  free(qp);
  return 0;
}

int ibv_destroy_cq(struct sim_cq *cq) {
  printf("ibv_destroy_cq() - Destroying Completion Queue\n");
  free(cq);
  return 0;
}

int ibv_dereg_mr(struct sim_mr *mr) {
  printf("ibv_dereg_mr() - Destroying Memory Region\n");
  free(mr);
  return 0;
}

int ibv_close_device(struct sim_context *context) {
  printf("ibv_close_device() - Closing device context\n");
  free(context);
  return 0;
}

int ibv_dealloc_pd(struct sim_pd *pd) {
  printf("ibv_dealloc_pd() - Deallocating protection domain\n");
  free(pd);
  return 0;
}
void cleanup_rdma_context(struct rdma_context *ctx) {
  printf("Cleaning up RDMA resources...\n");
  if (ctx->qp) {
    ibv_destroy_qp(ctx->qp);
  }
  if (ctx->cq) {
    ibv_destroy_cq(ctx->cq);
  }
  if (ctx->mr) {
    ibv_dereg_mr(ctx->mr);
  }
  if (ctx->buffer) {
    free(ctx->buffer);
    ctx->buffer = NULL;
    printf("Buffer at %p freed\n", (void *)ctx->buffer);
  }
  if (ctx->pd) {
    ibv_dealloc_pd(ctx->pd);
  }
  if (ctx->context) {
    ibv_close_device(ctx->context);
  }
  printf("RDMA resources cleaned up successfully!\n");
}

int setup_rdma_context(struct rdma_context *ctx) {
  int num_devices;
  printf("Setting up RDMA context...\n");
  // get list of devices
  struct sim_device **dev_list = ibv_get_device_list(&num_devices);
  if (!dev_list || num_devices <= 0) {
    fprintf(stderr, "Failed to get RDMA devices\n");
    return -1;
  }
  printf("\n");
  // open device
  ctx->context = ibv_open_device(dev_list[0]);
  if (!ctx->context) {
    fprintf(stderr, "Failed to open device\n");
    return -1;
  }
  printf("\n");
  // Allocate protection domain
  ctx->pd = ibv_alloc_pd(ctx->context);
  if (!ctx->pd) {
    fprintf(stderr, "Failed to allocate protection domain\n");
    return -1;
  }
  printf("\n");
  // Allocate data buffer
  ctx->buffer = malloc(BUFFER_SIZE);
  if (!ctx->buffer) {
    fprintf(stderr, "Failed to allocate buffer\n");
    return -1;
  }
  printf("Allocating data buffer...\n");
  printf("Buffer allocated at %p with size %d bytes\n", (void *)ctx->buffer,
         BUFFER_SIZE);
  printf("\n");
  // Register memory region
  ctx->mr = ibv_reg_mr(ctx->pd, ctx->buffer, BUFFER_SIZE,
                       IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_READ |
                           IBV_ACCESS_REMOTE_WRITE);
  if (!ctx->mr) {
    fprintf(stderr, "Failed to register memory region\n");
    return -1;
  }
  printf("\n");
  // Create completion queue
  ctx->cq = ibv_create_cq(ctx->context, 16, NULL, NULL, 0);
  if (!ctx->cq) {
    fprintf(stderr, "Failed to create completion queue\n");
    return -1;
  }
  printf("\n");
  // Create queue pair
  struct ibv_qp_init_attr qp_attr = {
      .send_cq = ctx->cq,
      .recv_cq = ctx->cq,
      .qp_type = IBV_QPT_RC, // Reliable Connection
      .cap = {.max_send_wr = 16,
              .max_recv_wr = 16,
              .max_send_sqe =
                  1, // scatter-gather entry, basically how many separate memory
                     // buffers can be used in a single op
              .max_recv_sqe = 1}};
  ctx->qp = ibv_create_qp(ctx->pd, &qp_attr);
  if (!ctx->qp) {
    fprintf(stderr, "Failed to create queue pair\n");
    return -1;
  }
  printf("\n");

  ibv_free_device_list(dev_list);
  printf("RDMA context setup successfully!\n");
  return 0;
}