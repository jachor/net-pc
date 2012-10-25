#ifndef jachor_ip_hw__
#define jachor_ip_hw__

#include "ip_internal.h"
#include <stdint.h>

void ip_rx_read(void *buf, uint16_t offset, uint16_t size);

ip_return_t ip_tx_begin(const uint8_t *address, uint16_t protocol);
void ip_tx_write(const void *buf, uint16_t offset, uint16_t length);
ip_return_t ip_tx_end(uint16_t length);

#endif
