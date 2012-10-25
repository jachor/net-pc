#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "ip_hw_pc.h"
#include "ip_hw.h"
#include "ip_internal.h"
#include "ip.h"

int main() {
    ip_hw_pc_init("tuneth");

    ip_parse_mac(ip_config.mac, "00:01:02:77:a4:26");
    ip_parse_addr(ip_config.address, "10.0.0.10");
    ip_parse_addr(ip_config.netmask, "255.0.0.0");

    while(1) {
        ip_hw_pc_fetch_packet();

        ip_handle();
    }

    return 0;
}

