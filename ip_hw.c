#include "ip_hw.h"
#include "enc28j60.h"
#include <string.h>

void ip_rx_read(void *buf, uint16_t offset, uint16_t size) {
    enc28j60_rx_read(buf, offset, size);
}

ip_return_t ip_tx_begin(const uint8_t *address, uint16_t protocol) {
    // czy wolny hw?
    if(enc28j60_tx_begin())
        return RETURN_NEED_TX;

    // wpisz dane
    ip_tx_write(address, 0, IP_MAC_SIZE);
    ip_tx_write(ip_config.mac, IP_MAC_SIZE, IP_MAC_SIZE);
    uint8_t buf[2];
    ip_write16(buf, protocol);
    ip_tx_write(buf, IP_MAC_SIZE*2, 2);
    return RETURN_FINISHED;
}

void ip_tx_write(const void *buf, uint16_t offset, uint16_t length) {
    enc28j60_tx_write(buf, offset, length);
}

ip_return_t ip_tx_end(uint16_t length) {
    // dopełnij zerami do 64 bajtów
    if(length < 64) {
        char buf[16];
        memset(buf, 0, sizeof(buf));
        while(length < 64) {
            uint16_t size = 64 - length;
            if(size > sizeof(buf))
                size = sizeof(buf);
            enc28j60_tx_write(buf, length, size);
            length += size;
        }
    }

    // wyślij
    enc28j60_tx_end(length);
    return RETURN_FINISHED;
}

