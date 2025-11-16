# dpdk-packet-pipeline

---

# Minimal Educational DPDK Project

This project is a **minimal, educational DPDK (Data Plane Development Kit) application** designed to help beginners understand DPDK's internal workings—without requiring any external NIC hardware.

The goal is to build a simple but complete packet-processing pipeline using DPDK, serving as a foundation for high-performance networking systems.

---

##  Project Overview: The DPDK Pipeline

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

##  What This Project Demonstrates

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

##  Why This Project Is Useful

This project is intentionally **simple** so you can quickly understand:

* How DPDK programs are **structured**
* How packet buffers (**mbufs**) move through the pipeline
* How to work with **queues** and memory **mempools**
* How to create high-speed networking applications
* How 5G UPF implementations use DPDK internally

## Flow chart

                             +----------------------+
                             |   Start Application  |
                             +----------+-----------+
                                        |
                                        v
                         +--------------+--------------+
                         |   rte_eal_init()           |
                         | - Parse EAL args           |
                         | - Init hugepages           |
                         | - Init lcores              |
                         | - Discover NICs            |
                         +--------------+--------------+
                                        |
                                        v
                         +--------------+--------------+
                         | Create Mempool             |
                         | rte_pktmbuf_pool_create()  |
                         +--------------+--------------+
                                        |
                                        v
                         +--------------+--------------+
                         | Configure Ethernet Port     |
                         | - rte_eth_dev_configure()   |
                         | - Setup RX queue            |
                         | - Setup TX queue            |
                         | - Start port                |
                         +--------------+--------------+
                                        |
                                        v
           +----------------------------+----------------------------+
           |                          Main Loop                      |
           |----------------------------------------------------------|
           | 1. RX = rte_eth_rx_burst()  <-- Receive packets          |
           | 2. For each packet:                                      |
           |       - Parse / print / inspect                          |
           | 3. TX = rte_eth_tx_burst()  <-- Transmit packets         |
           | 4. Free untransmitted buffers                            |
           +----------------------------------------------------------+
                                        |
                                        v
                         +--------------+--------------+
                         |   Stop Port & Cleanup       |
                         |   rte_eth_dev_stop()        |
                         |   rte_eth_dev_close()       |
                         +--------------+--------------+
                                        |
                                        v
                             +----------------------+
                             |     End Program      |
                             +----------------------+



## Architecture 


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


#  How to Run This Project (Step-by-Step Guide)

This section explains exactly how to run the DPDK application, starting from system preparation to executing the compiled binary. No assumptions. No missing steps. Everything is included.

---

##  1. System Requirements

Before running the project, ensure you have the following prerequisites installed and configured:

| Requirement | Details |
| :--- | :--- |
| **Linux Environment** | Ubuntu 20.04 / 22.04 recommended. |
| **Supported CPU** | Any modern Intel/AMD CPU with Hugepage support. |

### Installed Packages

Install necessary build tools and libraries:

```bash
sudo apt update
sudo apt install -y build-essential meson ninja-build \
                    libnuma-dev python3-pip
````

##  2. Install DPDK (if not already installed)

### Option A: Install via APT (Recommended for quick setup)

```bash
sudo apt install -y dpdk dpdk-dev
```

### Option B: Build from Source

```bash
wget [https://fast.dpdk.org/rel/dpdk-22.11.1.tar.xz](https://fast.dpdk.org/rel/dpdk-22.11.1.tar.xz)
tar -xf dpdk-22.11.1.tar.xz
cd dpdk-22.11.1
meson build
ninja -C build
sudo ninja -C build install
sudo ldconfig
```

##  3. Clone Your Repository

```bash
git clone [https://github.com/](https://github.com/)<your-username>/<repo-name>.git
cd <repo-name>
```

##  4. Build the Project

Assuming your project uses a simple Makefile structure:

```bash
make
```

This will generate the executable binary, typically located at `./build/dpdk-app`.

##  5. Allocate Hugepages (Required for DPDK)

DPDK must use hugepages for zero-copy buffer operations. Allocate 1024 pages of 2MB each:

```bash
sudo sysctl -w vm.nr_hugepages=1024
```

**Verification:**

```bash
grep Huge /proc/meminfo
```

##  6. Load Necessary Kernel Modules

DPDK requires specific user-space I/O drivers (VFIO or UIO).

### Option A — Recommended: VFIO

```bash
sudo modprobe vfio-pci
sudo modprobe vfio
```

### Option B — UIO (For older hardware/setups)

```bash
sudo modprobe uio
sudo modprobe igb_uio
```

##  7. Bind Your NIC (Optional)

Since this educational project works with virtual devices, binding a physical NIC is optional.

To allow DPDK to control a physical NIC instead of the Linux kernel:

1.  **Check NIC Status:**
    ```bash
    sudo dpdk-devbind.py --status
    ```
2.  **Bind the Device (Replace `0000:03:00.0` with your NIC's PCI address):**
    ```bash
    sudo dpdk-devbind.py -b vfio-pci 0000:03:00.0
    ```

##  8. Run the DPDK Application

The run command depends on whether you are using a virtual device or a physical NIC.

### ✔ A) Run using a Virtual Device (PCAP)

This method works without any physical NIC hardware and is perfect for offline lab testing.

```bash
sudo ./build/dpdk-app \
  --vdev=net_pcap0,rx_pcap=in.pcap,tx_pcap=out.pcap \
  --log-level=lib.eal:info \
  -l 0-1 -n 4
```

This command creates:

  * `in.pcap` → Used as the **RX input** stream.
  * `out.pcap` → Stores the **TX output** stream.

### ✔ B) Run using an Actual NIC

This requires a NIC bound using the `vfio-pci` or `igb_uio` driver (Step 7).

```bash
sudo ./build/dpdk-app -l 0-1 -n 4 -- -p 0x1
```

| Parameter | Meaning |
| :--- | :--- |
| `-l 0-1` | Use CPU logical cores 0 and 1. |
| `-n 4` | Use 4 memory channels (based on system configuration). |
| `-- -p 0x1` | Application-specific: enable port 0 (bitmask `0x1`). |

##  9. Verify Output

When the application runs successfully, you will see logs confirming initialization and packet processing:

```text
EAL: Detected 8 lcores
EAL: Probing VFIO driver
Mempool created with 8191 buffers
Port 0 initialized
RX packets: 32
Processing packet...
TX packets: 32
```

This confirms:

  * DPDK initialized correctly.
  * Mempool is working.
  * NIC queues are active.
  * Packets are being received and transmitted.

##  10. Stopping the Application

Press:

$$\text{Ctrl} + \text{C}$$

The application will execute its graceful shutdown sequence:

  * Stop the NIC queues.
  * Free allocated mbufs.
  * Release hugepages resources.
  * Exit cleanly.

-----
#  Conclusion:

This project provides a clear and practical introduction to **DPDK (Data Plane Development Kit)**, demonstrating exactly how high-performance packet-processing applications are built from the ground up.



Through this complete example, you have gained hands-on experience in the essential components of any DPDK application:

* **Initialization:** Initialize the DPDK **Environment Abstraction Layer (EAL)**.
* **NIC Management:** Configure a network interface with dedicated **RX/TX queues**.
* **Memory Management:** Allocate and manage **packet buffers (mbufs)** using **mempools**.
* **Core Logic:** Receive, process, and transmit packets in a **zero-copy, high-speed loop**.
* **Deployment:** Run DPDK applications using both physical NICs and virtual/pcap devices.
* **Validation:** Validate that DPDK is installed and functioning correctly on your system.

##  Next Steps and Advanced Systems

By understanding each core component—EAL, mempools, mbufs, NIC queues, and the packet loop—you now have a solid foundation for building more advanced networking systems such as:

* L2/L3 packet forwarders
* Firewalls and ACL engines
* GTP-U / PFCP processors for 5G User Plane
* High-speed traffic generators
* Intrusion detection/prevention systems (IDS/IPS)
* Custom protocol analyzers and test tools

This project acts as a **strong starting point** for anyone entering the fields of **telco, cloud, or high-performance networking**, and prepares you for scaling into complex real-world data-plane architectures.

You can now confidently **extend this project**, containerize it using Docker, and publish it to GitHub or LinkedIn as a complete and professional DPDK learning resource.
