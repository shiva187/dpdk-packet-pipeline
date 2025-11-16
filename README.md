# dpdk-packet-pipeline

---

# 📘 Minimal Educational DPDK Project

This project is a **minimal, educational DPDK (Data Plane Development Kit) application** designed to help beginners understand DPDK's internal workings—without requiring any external NIC hardware.

The goal is to build a simple but complete packet-processing pipeline using DPDK, serving as a foundation for high-performance networking systems.

---

## 🏗️ Project Overview: The DPDK Pipeline

The project implements a full packet-processing pipeline covering all essential steps:

1.  **DPDK installation & environment setup**
2.  **Hugepages configuration**
3.  **NIC binding** (physical or virtual interface)
4.  **EAL initialization**
5.  **Mempool creation**
6.  **RX/TX queue setup**
7.  **Main packet-processing loop**
8.  **Graceful cleanup**

### Foundation for High-Performance Systems

This project forms a robust base template for building high-performance systems such as:

* 5G User Plane Function (UPF)
* Virtual switches (vSwitch)
* Packet gateways
* Firewalls and load balancers
* Custom protocol testing tools

The code demonstrates how **DPDK bypasses the Linux kernel** to achieve very high packet throughput using **user-space drivers** and **zero-copy buffers**.

---

## 🎯 What This Project Demonstrates

### 1. Full DPDK Initialization Sequence

You learn how the application:

* Reserves **hugepage memory**
* Initializes CPU cores (**lcores**)
* Maps network devices
* Sets up necessary memory zones

### 2. Packet Buffer Pool Creation

Using `rte_pktmbuf_pool_create()`, the project allocates a **mempool** of **mbufs** (message buffers) used to hold all Receive (RX) and Transmit (TX) packets.

### 3. NIC / Virtual NIC Configuration

The project supports configuration even without a physical NIC, including:

* vEth pairs
* TAP/PCAP-based NICs
* VFIO user-space drivers

It configures the following steps:

* Device initialization
* RX queue setup
* TX queue setup
* Starting the device

### 4. Packet Processing Loop (Core Feature)

A continuous pipeline runs on a dedicated core, demonstrating the bare minimum requirement of any DPDK-based data plane:

1.  **Receive packets:** (`rte_eth_rx_burst`)
2.  **Process:** (Inspect/print/update packet data)
3.  **Transmit packets:** (`rte_eth_tx_burst`)

### 5. Clean Shutdown

On exit, the project ensures a graceful shutdown by:

* Safely stopping NIC queues
* Closing the network device
* Releasing all allocated memory

---

## 🧠 Why This Project Is Useful

This project is intentionally **simple** so you can quickly understand:

* How DPDK programs are **structured**
* How packet buffers (**mbufs**) move through the pipeline
* How to work with **queues** and memory **mempools**
* How to create high-speed networking applications
* How 5G UPF implementations use DPDK internally

This acts as a base template for building more advanced high-performance networking applications.


                       +---------------------------+
                       |     DPDK Application      |
                       |     (main.c program)      |
                       +------------+--------------+
                                    |
                                    v
                     +--------------+----------------+
                     |     DPDK EAL Initialization   |
                     |  - Hugepages                  |
                     |  - Memory reservation         |
                     |  - Core/lcore init            |
                     |  - PCI device discovery       |
                     +--------------+----------------+
                                    |
                                    v
                     +--------------+----------------+
                     |      Mempool (mbuf pool)      |
                     |  rte_pktmbuf_pool_create()    |
                     +--------------+----------------+
                                    |
                                    v
                     +--------------+----------------+
                     |        NIC / vNIC Setup       |
                     |  rte_eth_dev_configure()      |
                     |  rte_eth_rx_queue_setup()     |
                     |  rte_eth_tx_queue_setup()     |
                     |  rte_eth_dev_start()          |
                     +--------------+----------------+
                                    |
                                    v
     +------------------------------+------------------------------+
     |                                                             |
     |                      PACKET PROCESSING LOOP                 |
     |                                                             |
     |   while (1) {                                               |
     |       RX = rte_eth_rx_burst();   <-- Receive packets        |
     |       for each packet:                                      |
     |            - Parse / Inspect                                |
     |            - Modify (optional)                              |
     |            - Forward                                        |
     |       TX = rte_eth_tx_burst();   <-- Transmit packets       |
     |   }                                                         |
     |                                                             |
     +------------------------------+------------------------------+
                                    |
                                    v
                     +--------------+----------------+
                     |      Port Stop / Cleanup      |
                     |  rte_eth_dev_stop()           |
                     |  rte_eth_dev_close()          |
                     |  Release mempool              |
                     +--------------+----------------+

