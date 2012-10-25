net-pc
======

Simple implementation of IP running on host computer via TUN device. Does not fully follow RFC.

Assumptions:
 * no dynamic memory allocation
 * little memory use
 * no IP fragmentation support

Use
---

1. Compile by typing:

        make

2. Setup TUN device (code requires tuneth and 10.0.0.*/24 by default):

        tunctl -u <user> -g <group> -t tuneth
        ifconfig tuneth 10.0.0.1/24 up

3. Run code (as user specified above):

        ./net-pc

4. Test (only ICMP echo request {aka. ping} works):

        ping 10.0.0.10