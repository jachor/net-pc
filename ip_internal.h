#ifndef jachor_ip_internal__
#define jachor_ip_internal__

#define PACKED __attribute__((packed))

#include "ip.h"
#include <stdint.h>

#define MAC_LEN  (6)
extern const uint8_t ip_mac_broadcast[IP_MAC_SIZE];
#define IP_LEN   (4)


/* 
 * nagłówek ethernet
 */
struct eth_header {
    uint8_t target[MAC_LEN];
    uint8_t sender[MAC_LEN];
    uint8_t protocol[2];
} PACKED;
#define ETH_HEADER_SIZE (6+6+2)

#define ETH_PROTO_ARP   (0x0806)
#define ETH_PROTO_IP    (0x0800)

/*
 * pakiet arp
 */
#define ARP_HTYPE_ETHERNET  (1)
#define ARP_PTYPE_IP        (0x0800)
#define ARP_OPER_REQUEST    (1)
#define ARP_OPER_RESPONSE   (2)
struct arp_message {
    uint8_t htype[2];       // hw type
    uint8_t ptype[2];       // proto type
    uint8_t hlen;           // hw length
    uint8_t plen;           // proto length
    uint8_t oper[2];        // operation
    uint8_t sha[MAC_LEN];   // sender hw addr
    uint8_t spa[IP_LEN];    // sender proto addr
    uint8_t tha[MAC_LEN];   // target hw addr
    uint8_t tpa[IP_LEN];    // target proto addr
} PACKED;
#define ARP_MESSAGE_SIZE    (28)

/*
 * pakiet IP
 */
struct ip_header {
    uint8_t version_ihl;    // version(0-3), Internet header length(4-7)
    uint8_t tos;            // type of service
    uint8_t length[2];      // total length
    uint8_t ident[2];       // identification
    uint8_t offset[2];      // flags(0-3), offset(4-15)
    uint8_t ttl;
    uint8_t protocol;
    uint8_t checksum[2];
    uint8_t sender[4];      // source address
    uint8_t target[4];      // destination address
    uint8_t options[0];     // optional
} PACKED;
#define IP_HEADER_SIZE      (20) // stosowany rozmiar

#define IP_PROTO_ICMP       (1)
#define IP_PROTO_TCP        (6)
#define IP_PROTO_UDP        (17)

/*
 * zwracane wartości
 */
typedef int ip_return_t;
#define RETURN_FINISHED     (0)
#define RETURN_NEED_TX      (1)

/*
 * suma kontrolna IP (RFC 1071)
 */
#include "ip_checksum.h"

/*
 * CRC16
 */
#define IP_CRC16_INIT       (0)
void ip_crc16(uint16_t *crc, const void *data, uint16_t length);

/*
 * informacje
 */
extern uint8_t ip_sender_mac[IP_MAC_SIZE];
extern uint8_t ip_sender_addr[IP_ADDR_SIZE];

/*
 * protokoły
 */
// obsługuję ramkę ethernet
ip_return_t ip_handle();
// obsługuję ARP
ip_return_t ip_handle_arp();
// tworzy ramkę IP do nadania (bez uzupełniania ethernet
void ip_tx_header(const uint8_t *target, uint8_t protocol, uint16_t payload_length);
// obsługuję ramkę IP
ip_return_t ip_handle_ip();
// obsługuję ramkę ICMP
ip_return_t ip_handle_icmp(uint16_t offset, uint16_t length);
// obsługuje ramkę IP
ip_return_t ip_handle_tcp(uint16_t offset, uint16_t length);

#endif
