#include "ip_hw_pc.h"
#include "ip_hw.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <poll.h>

static
int tun_alloc(const char *dev, int flags) {
    const char *clonedev = "/dev/net/tun";

    int fd = open(clonedev, O_RDWR);
    if(fd < 0) {
        errno = -fd;
        perror("Unable to open TUN clone device");
        return -1;
    }

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(struct ifreq));
    ifr.ifr_flags = flags;
    if(dev)
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);

    int res;
    if((res = ioctl(fd, TUNSETIFF, (void*)&ifr)) < 0) {
        errno = -res;
        (void) close(fd);
        perror("Unable to set TUN device flags");
        return -1;
    }

    fprintf(stderr, "Got TUN/TAP device %s\n", ifr.ifr_name);

    return fd;
}

static int ip_hw_pc_fd = -1;
static char ip_hw_pc_rxbuf[1600];
static char ip_hw_pc_txbuf[1600];

void ip_hw_pc_init(const char *dev_name) {
    ip_hw_pc_fd = tun_alloc(dev_name, IFF_TAP | IFF_NO_PI);
    if(ip_hw_pc_fd < 0) {
        fprintf(stderr, "Unable to open TUN device\n");
        exit(1);
    }
}

void ip_hw_pc_shutdown() {
}

int ip_hw_pc_has_packet() {
    struct pollfd pfd;
    pfd.fd = ip_hw_pc_fd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    if(poll(&pfd, 1, 0) < 0) {
        perror("poll");
        exit(1);
    }
    return (pfd.revents & POLLIN)?1:0;
}

void ip_hw_pc_fetch_packet() {
    int len = read(ip_hw_pc_fd, ip_hw_pc_rxbuf, sizeof(ip_hw_pc_rxbuf));
    if(len<0) {
        errno = -len;
        perror("read");
    }
}

void ip_rx_read(void *buf, uint16_t offset, uint16_t size) {
    if((int)offset+size >= sizeof(ip_hw_pc_rxbuf)) {
        fprintf(stderr, "Read past end of rx buffer\n");
        abort();
    }
    memcpy(buf, ip_hw_pc_rxbuf+offset, size);
}

ip_return_t ip_tx_begin(const uint8_t *address, uint16_t protocol) {
    memcpy(ip_hw_pc_txbuf, address, 6);
    memcpy(ip_hw_pc_txbuf+6, ip_config.mac, 6);
    uint8_t buf[2];
    ip_write16(buf, protocol);
    memcpy(ip_hw_pc_txbuf+12, buf, 2);
    return RETURN_FINISHED;
}
void ip_tx_write(const void *buf, uint16_t offset, uint16_t length) {
    if((int)offset+length >= sizeof(ip_hw_pc_txbuf)) {
        fprintf(stderr, "Read past end of rx buffer\n");
        abort();
    }
    memcpy(ip_hw_pc_txbuf+offset, buf, length);
}
ip_return_t ip_tx_end(uint16_t length) {
    if(length < 64) {
        memset(ip_hw_pc_txbuf+length, 0, 64-length);
    }
    int len = write(ip_hw_pc_fd, ip_hw_pc_txbuf, length);
    if(len < 0)
        perror("write");
    return RETURN_FINISHED;
}

