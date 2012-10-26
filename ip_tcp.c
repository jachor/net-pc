#include "ip_internal.h"
#include "ip_hw.h"
#include "ip_cdll.h"

struct tcp_header {
    uint8_t sender[2];      // port źródłowy
    uint8_t target[2];      // port docelowy
    uint8_t seqno[4];       // numer sekwencyjny
    uint8_t ackno[4];       // numer potwierdzenia
    uint8_t hlen;           // 0-3-dl naglowka
    uint8_t flags;
    uint8_t window[2];      // rozmiar okna
    uint8_t checksum[2];
    uint8_t priority[2];
    uint8_t options[0];     // opcje jak trzeba
} PACKED;

struct ip_tcp_socket {
    struct ip_cdll_entry list;
    uint8_t mac[IP_MAC_SIZE];
    uint8_t addr[IP_ADDR_SIZE];
    uint8_t port[2];
};

struct ip_cdll_entry socket_list = {&socket_list,&socket_list};

ip_return_t ip_handle_tcp(uint16_t offset, uint16_t length) {
    struct tcp_header hdr;
    ip_rx_read(&hdr, offset, length);

    uint16_t csum;
    ip_checksum_init(&csum);

    return RETURN_FINISHED;
}

