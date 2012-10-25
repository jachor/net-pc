#include "ip_internal.h"
#include "ip_hw.h"
#include <stdio.h>

struct icmp_header {
    uint8_t type;
    uint8_t code;
    uint8_t checksum[2];
    uint8_t quench[4];
} PACKED;
#define ICMP_HEADER_SIZE        (8)

#define ICMP_TYPE_ECHO_REQUEST  (8)
#define ICMP_TYPE_ECHO_REPLY    (0)

ip_return_t ip_handle_icmp(uint16_t offset, uint16_t length) {
    struct icmp_header hdr;
    ip_rx_read(&hdr, offset, sizeof(hdr));

    if(hdr.type != ICMP_TYPE_ECHO_REQUEST)
        return RETURN_FINISHED; // tylko PING

    ip_return_t r = ip_tx_begin(ip_sender_mac, ETH_PROTO_IP);
    if(r != RETURN_FINISHED)
        return r;

    ip_tx_header(ip_sender_addr, IP_PROTO_ICMP, length);

    uint16_t csum_rx;
    ip_checksum_init(&csum_rx);
    ip_checksum_update(&csum_rx, &hdr, sizeof(hdr));

    /* 
     * zmodyfikuj nagłówek na odpowiedź
     */
    hdr.type = ICMP_TYPE_ECHO_REPLY;
    hdr.checksum[0] = 0;
    hdr.checksum[1] = 0;

    uint16_t csum_tx;
    ip_checksum_init(&csum_tx);
    ip_checksum_update(&csum_tx, &hdr, sizeof(hdr));

    /* 
     * skopiuj dane
     * po drodze oblicz sumę kontrolną do nadawania i odbierania.
     */
    uint16_t dlen = length - ICMP_HEADER_SIZE;
    uint16_t rxoff = offset + ICMP_HEADER_SIZE;
    uint16_t txoff = ETH_HEADER_SIZE + IP_HEADER_SIZE + ICMP_HEADER_SIZE;
    while(dlen > 0) {
        uint8_t buf[16];
        uint16_t plen = dlen;
        if(plen > sizeof(buf))
            plen = sizeof(buf);
        ip_rx_read(buf, rxoff, plen);
        ip_tx_write(buf, txoff, plen);
        ip_checksum_update(&csum_rx, buf, plen);
        ip_checksum_update(&csum_tx, buf, plen);
        rxoff += plen; txoff += plen; dlen -= plen;
    }

    ip_checksum_finish(&csum_tx);
    ip_checksum_finish(&csum_rx);

    if(csum_rx != 0)
        return RETURN_FINISHED; // zla suma kontrolna odbieranego pakietu

    ip_write16(hdr.checksum, csum_tx);
    ip_tx_write(&hdr, ETH_HEADER_SIZE+IP_HEADER_SIZE, sizeof(hdr));
    ip_tx_end(length + ETH_HEADER_SIZE + IP_HEADER_SIZE);

    return RETURN_FINISHED;
}

