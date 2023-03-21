#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

/*
 *  Getting IP address from a network interface
 *  reference: http://www.geekpage.jp/en/programming/linux-network/get-ipaddr.php 
 * 
 */

char * get_wlan0_ip_addr();
 
int main()
{
    char * IP_addr = get_wlan0_ip_addr();

    printf("%s\n", IP_addr);
    
    return 0;
}

char * get_wlan0_ip_addr() {
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    
    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;
    
    /* I want IP address attached to "wlan0" */
    strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);
    
    ioctl(fd, SIOCGIFADDR, &ifr);
    
    close(fd);
    
    return (inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
}


    // int fd;
    // struct ifreq ifr;

    // fd = socket(AF_INET, SOCK_DGRAM, 0);
    
    // /* I want to get an IPv4 IP address */
    // ifr.ifr_addr.sa_family = AF_INET;
    
    // // I want IP address attached to "wlan0"
    // strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);
    
    // ioctl(fd, SIOCGIFADDR, &ifr);
    
    // close(fd);
    
    // // display result
    // printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
