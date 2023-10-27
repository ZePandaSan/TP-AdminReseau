#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

struct iphdr fill_ip_header() {
    struct iphdr ip_header;

    ip_header.ihl = sizeof(struct iphdr) / 4; // Size of the header divided by 4
    ip_header.version = 4; // IPv4
    ip_header.tos = 0; // Type of service
    ip_header.tot_len = htons(sizeof(struct iphdr)); // Total length of the packet
    ip_header.id = htons(1234); // Arbitrary identifier
    ip_header.frag_off = 0; // Fragment offset
    ip_header.ttl = 255; // Time to live
    ip_header.protocol = IPPROTO_ICMP; // Protocol type
    ip_header.check = 0; // Checksum (to be computed)
    ip_header.saddr = inet_addr("192.168.1.1"); // Source address
    ip_header.daddr = inet_addr("192.168.1.2"); // Destination address

    return ip_header;
}

struct icmphdr fill_icmp_header() {
    struct icmphdr icmp_header;

    icmp_header.type = ICMP_ECHO;
    icmp_header.code = 0;
    icmp_header.checksum = 0; // You should compute the checksum
    icmp_header.un.echo.id = htons(1234); // Replace with the desired ID
    icmp_header.un.echo.sequence = htons(1); // Replace with the desired sequence

    return icmp_header;
}   

void display(char *buffer, int length) {
    for (int i = 0; i < length; ++i) {
        if (i > 0 && i % 16 == 0) {
            printf("\n");
        }
        printf("%02x", (unsigned char)buffer[i]);
        if (i % 2 != 0) {
            printf(" ");
        }
    }
    printf("\n");
}

int main() {
    char buffer[50];
    for (int i = 0; i < 50; i++) {
        buffer[i] = i;
    }
    display(buffer, 50);

    struct iphdr ip_header = fill_ip_header();
    printf("IP Header Display:\n");
    display((char *)&ip_header, sizeof(ip_header));

    struct icmphdr icmp_header = fill_icmp_header();
    printf("\nICMP Header Display:\n");
    display((char *)&icmp_header, sizeof(icmp_header));
    return 0;
}
