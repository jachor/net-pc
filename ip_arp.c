#include "ip_internal.h"
#include "ip_hw.h"
#include <string.h>
#include <stdio.h>

#define ARP_CACHE_SIZE      (8)

ip_return_t ip_handle_arp() {
    struct arp_message msg;
    ip_rx_read(&msg, ETH_HEADER_SIZE, ARP_MESSAGE_SIZE);

    // czy zgadzają się typy protokołów?
    if(ip_read16(msg.htype) != ARP_HTYPE_ETHERNET)
        return RETURN_FINISHED;
    if(ip_read16(msg.ptype) != ARP_PTYPE_IP)
        return RETURN_FINISHED;
    if(msg.hlen != IP_MAC_SIZE || msg.plen != IP_ADDR_SIZE)
        return RETURN_FINISHED;

    // TODO: dodaj msg.spa, msg.sha do tablicy ARP
    printf("arp ip .%d = mac :%02x\n", msg.spa[3], msg.sha[5]);

    // czy to do mnie?
    if(memcmp(msg.tpa, ip_config.address, IP_ADDR_SIZE) != 0)
        return RETURN_FINISHED;

    if(ip_read16(msg.oper) != ARP_OPER_REQUEST)
        return RETURN_FINISHED;

    printf("arp tx?\n");
    // TODO: zacznij wysylanie
    ip_return_t r = ip_tx_begin(msg.sha, ETH_PROTO_ARP);
    if(r != RETURN_FINISHED)
        return r;

    // ustaw docelowe na źródłowe adresy
    memcpy(msg.tpa, msg.spa, IP_ADDR_SIZE);
    memcpy(msg.tha, msg.sha, IP_MAC_SIZE);
    // wpisz swoje dane w źródłowych adresach
    memcpy(msg.spa, ip_config.address, IP_ADDR_SIZE);
    memcpy(msg.sha, ip_config.mac, IP_MAC_SIZE);
    // ustaw kod operacji na odpowiedź
    ip_write16(msg.oper, ARP_OPER_RESPONSE);

    ip_tx_write(&msg, ETH_HEADER_SIZE, sizeof(msg));
    printf("arp tx!\n");
    return ip_tx_end(sizeof(msg) + ETH_HEADER_SIZE);
}

ip_return_t ip_query_arp(const uint8_t *ip_addr) {
    // zacznij wysylanie
    ip_return_t r = ip_tx_begin(ip_mac_broadcast, ETH_PROTO_ARP);
    if(r != RETURN_FINISHED)
        return r;

    struct arp_message msg;
    ip_write16(msg.htype, ARP_HTYPE_ETHERNET);
    ip_write16(msg.ptype, ARP_PTYPE_IP);
    msg.hlen = IP_MAC_SIZE;
    msg.plen = IP_ADDR_SIZE;
    memcpy(msg.sha, ip_config.mac, IP_MAC_SIZE);
    memcpy(msg.spa, ip_config.address, IP_ADDR_SIZE);
    memset(msg.tha, 0, IP_MAC_SIZE);
    memcpy(msg.tpa, ip_addr, IP_MAC_SIZE);
    ip_write16(msg.oper, ARP_OPER_REQUEST);

    ip_tx_write(&msg, ETH_HEADER_SIZE, sizeof(msg));
    printf("arp tx req!\n");
    return ip_tx_end(sizeof(msg) + ETH_HEADER_SIZE);
}


