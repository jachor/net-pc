#ifndef jachor_ip__
#define jachor_ip__

#include <stdint.h>

#define IP_MAC_SIZE     (6)
#define IP_ADDR_SIZE    (4)
#define IP_MAX_ADDR_TEXT (4*4)
#define IP_MAX_MAC_TEXT (6*3)

struct ip_config {
    uint8_t mac[IP_MAC_SIZE];
    uint8_t address[IP_ADDR_SIZE];
    uint8_t netmask[IP_ADDR_SIZE];
    uint8_t gateway[IP_ADDR_SIZE];
};

extern struct ip_config ip_config;

char ip_parse_addr(uint8_t *out, const char *addr);
char ip_parse_mac(uint8_t *out, const char *addr);
int ip_print_addr(char *buf, const uint8_t *addr);
int ip_print_mac(char *buf, const uint8_t *mac);

uint16_t ip_read16(const uint8_t *ptr);
uint32_t ip_read32(const uint8_t *ptr);
void ip_write16(uint8_t *buf, uint16_t value);
void ip_write32(uint8_t *buf, uint32_t value);

#endif
