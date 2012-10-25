#ifndef jachor_ip_crc_h__
#define jachor_ip_crc_h__

#include "ip.h"

static inline
void ip_checksum_init(uint16_t *checksum) {
    *checksum = 0;
}

static inline
void ip_checksum_update(uint16_t *checksum, const void *data, uint16_t length) {
    uint32_t csum = *checksum;
    const uint8_t *d = (const uint8_t*)data;
    while(length>=2) {
        csum += ip_read16(d);
        length -= 2; d+=2;
    }
    if(length)
        csum += (uint16_t)(*d) << 8;
    csum = (csum&0xffff) + (csum >> 16);
    *checksum = csum;
}

static inline
void ip_checksum_finish(uint16_t *checksum) {
    *checksum = ~(*checksum);
}

static inline
uint16_t ip_checksum(const void *data, uint16_t length) {
    uint16_t checksum;
    ip_checksum_init(&checksum);
    ip_checksum_update(&checksum, data, length);
    ip_checksum_finish(&checksum);
    return checksum;
}

#endif

