#include <rte_eal.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include <rte_ring.h>
#include <rte_ethdev.h>
#include <stdio.h>

#define NB_MBUFS 8192
#define MBUF_CACHE_SIZE 256
#define RING_SIZE 1024

int main(int argc, char **argv) {
    int ret = rte_eal_init(argc, argv);
    if (ret < 0) rte_exit(EXIT_FAILURE, "EAL init failed\n");

    printf("✅ DPDK EAL initialized successfully!\n");

    struct rte_mempool *mbuf_pool = rte_pktmbuf_pool_create("MBUF_POOL",
                                NB_MBUFS, MBUF_CACHE_SIZE, 0,
                                RTE_MBUF_DEFAULT_BUF_SIZE,
                                rte_socket_id());

    if (mbuf_pool == NULL)
        rte_exit(EXIT_FAILURE, "Cannot create mbuf pool\n");

    printf("✅ Mempool created successfully!\n");

    struct rte_ring *ring = rte_ring_create("LOOP_RING", RING_SIZE,
                            rte_socket_id(), RING_F_SP_ENQ | RING_F_SC_DEQ);

    if (ring == NULL)
        rte_exit(EXIT_FAILURE, "Cannot create ring\n");

    printf("✅ Ring created successfully!\n");

    struct rte_mbuf *pkt = rte_pktmbuf_alloc(mbuf_pool);
    char *data = rte_pktmbuf_append(pkt, 64);
    snprintf(data, 64, "Hello from DPDK!");

    printf("📤 Enqueuing packet: %s\n", data);
    rte_ring_enqueue(ring, pkt);

    struct rte_mbuf *recv_pkt;
    if (rte_ring_dequeue(ring, (void **)&recv_pkt) == 0) {
        char *recv_data = rte_pktmbuf_mtod(recv_pkt, char *);
        printf("📥 Dequeued packet: %s\n", recv_data);
        rte_pktmbuf_free(recv_pkt);
    }

    printf("✅ DPDK loopback test completed successfully.\n");
    return 0;
}

