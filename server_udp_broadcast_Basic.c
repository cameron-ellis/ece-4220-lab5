/* 	Name       : 	server_udp_broadcast.c
	Author     : 	Luis A. Rivera
	Description: 	Simple server (broadcast)
					ECE4220/7220		*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netdb.h>

#define MSG_SIZE 40			// message size
#define IP_MAX_SIZE 20      // max size of IP address

char * get_wlan0_ip_addr();
void ip_to_int_arr(char * IP_addr);

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
   int sock, length, n;
   int boolval = 1;			// for a socket option
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in addr;
   char buffer[MSG_SIZE];	// to store received messages or messages to be sent.
   char rasp_ip_addr[IP_MAX_SIZE]; // to store IP address of raspberry pi
   int rasp_ip_arr[4]; // integer array to hold parts of IP address for raspberry pi

   if (argc < 2)
   {
	  printf("usage: %s port\n", argv[0]);
      exit(0);
   }

   sock = socket(AF_INET, SOCK_DGRAM, 0); // Creates socket. Connectionless.
   if (sock < 0)
	   error("Opening socket");

   length = sizeof(server);			// length of structure
   bzero(&server,length);			// sets all values to zero. memset() could be used
   server.sin_family = AF_INET;		// symbol constant for Internet domain
   server.sin_addr.s_addr = INADDR_ANY;		// IP address of the machine on which
											// the server is running
   server.sin_port = htons(atoi(argv[1]));	// port number

   // binds the socket to the address of the host and the port number
   if (bind(sock, (struct sockaddr *)&server, length) < 0)
       error("binding");

   // change socket permissions to allow broadcast
   if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &boolval, sizeof(boolval)) < 0)
    {
   		printf("error setting socket options\n");
   		exit(-1);
   	}

   // Determine the IP of the raspberry pi
   strncpy(rasp_ip_addr, get_wlan0_ip_addr(), MSG_SIZE);

   // Split up IP addr of raspberry pi into integers
   ip_to_int_arr(rasp_ip_addr);

   fromlen = sizeof(struct sockaddr_in);	// size of structure

   while (1)
   {
	   // bzero: to "clean up" the buffer. The messages aren't always the same length...
	   bzero(buffer,MSG_SIZE);		// sets all values to zero. memset() could be used

	   // receive from a client
	   n = recvfrom(sock, buffer, MSG_SIZE, 0, (struct sockaddr *)&addr, &fromlen);
       if (n < 0)
    	   error("recvfrom");

       // Compare strings to see if VOTE or WHOIS message was sent 
       int Case1 = strncmp(buffer, "VOTE", 5);
       int Case2 = strncmp(buffer, "WHOIS", 5);
       
       if (Case1 == 0) {
            srand(time(0));
            int rand_num = rand() % 9;
            char message[50] = {"# "};
            char copy_rasp_ip = get_wlan0_ip_addr();
            strcat(message, copy_rasp_ip);
       }
       printf("Received a datagram. It says: %s", buffer);

       // To send a broadcast message, we need to change IP address to broadcast address
       // If we don't change it (with the following line of code), the message
       // would be transmitted to the address from which the message was received.
	   // You may need to change the address below (check ifconfig)
       addr.sin_addr.s_addr = inet_addr("128.206.19.255");		// broadcast address

       n = sendto(sock, "Got a message. Was it from you?\n", 32, 0,
    		      (struct sockaddr *)&addr, fromlen);
       if (n  < 0)
    	   error("sendto");
   }

   return 0;
 }

// get_wlan0_ip_addr
/*  Function for getting wlan0 IP address of raspberry pi 
    and returning as a char pointer 
*/
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
        rasp_ip_arr[i] = atoi(token);
        i++;
        token = strtok(NULL, delim);
    }

    return;
}