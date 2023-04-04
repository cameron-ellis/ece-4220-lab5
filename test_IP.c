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

int ip_arr[4];
char * get_wlan0_ip_addr();
void ip_to_int_arr(char * IP_addr);

int main()
{
    char IP_addr[40]; 
    strncpy(IP_addr, get_wlan0_ip_addr(), 40);

    char message[50] = {"# "};

    printf("%s\n", message);
    
    ip_to_int_arr(IP_addr);
    
    for (int i = 0; i < 4; i++)
    {
        printf("%d\n", ip_arr[i]);
    }
    
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

// ip_to_int_arr
/* Function to convert raspberry pi IP address into
   integer array containing all the parts of the IP
*/
void ip_to_int_arr(char * IP_addr) {
    const char delim[2] = ".";
    char *token;

    token = strtok(IP_addr, delim);
    int i = 0;

    while (token != NULL)
    {
        ip_arr[i] = atoi(token);
        i++;
        token = strtok(NULL, delim);
    }

    return;
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
