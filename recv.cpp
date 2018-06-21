#include <iostream>
#include <cstdio>
#include <cstring>

extern "C"{
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <errno.h>
}

using namespace std;

static int fd;

struct timer{
    timeval s,e;
    timer(){
        gettimeofday(&s,NULL);
    }
    void stop(){
        gettimeofday(&e,NULL);
    }
    void print(){
        int sec = e.tv_sec - s.tv_sec;
        int usec = e.tv_usec - s.tv_usec;
        cout << "Elapsed time: " << sec*1000+(usec/1000.0) << " ms" << endl;
    }
};

void perror_exit(const char* str){
    perror(str);
    exit(EXIT_FAILURE);
}

void create_udp(const char *port){
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd==-1){
        perror_exit("socket");
    }
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(fd,(sockaddr*)&addr,sizeof(addr))!=0){
        perror_exit("bind");
    }
}


int main (int argc, char *argv[]){
    if(argc!=2){
        cerr << "Usage: recv port";
        exit(EXIT_FAILURE);
    }
    create_udp(argv[1]);
    char buf[100];
    bzero(buf,sizeof(buf));
    recv(fd,buf,sizeof(buf),0);
    while(1){
        timer t;
        recv(fd,buf,sizeof(buf),0);
        t.stop();
        t.print();
    }
}
