#include "rdma_sim.h"

#define BUFFER_SIZE 1024

// simulate device list (mock hardware)
static struct sim_device sim_devices[] = {
    {"sim_rdma_device_0", 0},
    {"sim_rdma_device_1", 1}
};

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
    printf("Device 0: %s, ID: %d\n", device_ptrs[0]->name, device_ptrs[0]->device_id);
    printf("Device 1: %s, ID: %d\n", device_ptrs[1]->name, device_ptrs[1]->device_id);

    return device_ptrs;
}

struct sim_context *ibv_open_device(struct sim_device *device) {
    printf("ibv_open_device() - Opening device: %s\n", device->name);

    struct sim_context *ctx = malloc(sizeof(struct sim_context));
    ctx->device = device;
    ctx->is_open = 1;

    printf("Device opened successfully: %s\n", device->name);
    printf("Context created at %p\n", (void*)ctx);

    return ctx;
}

void ibv_free_device_list(struct sim_device **list) {
    printf("ibv_free_device_list() - Freeing device list\n");
    // In a real implementation, we would free each device structure
    // Here we just print a message as we are using static devices
}

int setup_rdma_context(struct rdma_context *ctx) {
    int num_devices;
    printf("Setting up RDMA context...\n");
    // get list of devices
    struct sim_device **dev_list = ibv_get_device_list(&num_devices);
    if(!dev_list || num_devices <= 0) {
        fprintf(stderr, "Failed to get RDMA devices\n");
        return -1;
    }
    // open device
    ctx->context = ibv_open_device(dev_list[0]);
    if(!ctx->context) {
        fprintf(stderr, "Failed to open device\n");
        return -1;
    }
    ibv_free_device_list(dev_list);
}