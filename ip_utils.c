#include "ip.h"
#include <stdio.h>

char ip_parse_addr(uint8_t *out, const char *addr) {
    uint8_t i;
    for(i=0;i<4;i++) {
        // kropka
        if(i!=0 && *(addr++)!='.')
            return 1;
        // cyfry
        if(*addr < '0' || *addr > '9')
            return 1;
        uint16_t v = 0;
        while(*addr >= '0' && *addr <= '9')
            v = v*10 + (*(addr++) - '0');
        if(v>255)
            return 1;
        out[i] = v;
    }
    if(*addr)
        return 1;
    return 0;
}

char ip_parse_mac(uint8_t *out, const char *addr) {
    uint8_t i;
    for(i=0;i<6;i++) {
        if(i!=0 && *addr==':')
            addr++;
        uint8_t j;
        uint8_t v = 0;
        for(j=0;j<2;j++) {
            v <<= 4;
            if(*addr >= '0' && *addr <= '9')
                v |= *addr - '0';
            else if(*addr >= 'a' && *addr <= 'f')
                v |= *addr - 'a' + 10;
            else if(*addr >= 'A' && *addr <= 'F')
                v |= *addr - 'A' + 10;
            else
                return 1;
            addr++;
        }
        out[i] = v;
    }
    if(*addr)
        return 1;
    return 0;
}

int ip_print_addr(char *buf, const uint8_t *addr) {
    return sprintf(buf, "%d.%d.%d.%d", addr[0], addr[1], addr[2], addr[3]);
}

int ip_print_mac(char *buf, const uint8_t *mac) {
    return sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
}

uint16_t ip_read16(const uint8_t *ptr) {
    return (uint16_t)ptr[0] << 8 | ptr[1];
}
uint32_t ip_read32(const uint8_t *ptr) {
    return (uint32_t)ip_read16(ptr+0) << 16 | ip_read16(ptr+2);
}
void ip_write16(uint8_t *buf, uint16_t value) {
    buf[0] = value >> 8;
    buf[1] = value & 0xff;
}
void ip_write32(uint8_t *buf, uint32_t value) {
    ip_write16(buf+0, value & 0xffff);
    ip_write16(buf+2, value >> 16);
}

const uint8_t ip_mac_broadcast[IP_MAC_SIZE] = {0xff,0xff,0xff,0xff,0xff,0xff};

