CC = gcc
APP = dpdk_loopback
CFLAGS += -O3 -march=native
LDFLAGS += $(shell pkg-config --libs --cflags libdpdk)

all:
	$(CC) $(CFLAGS) dpdk_loopback.c -o $(APP) $(LDFLAGS)

clean:
	rm -f $(APP)

