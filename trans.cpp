#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include "protocol.hpp"

extern "C"{
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
}

using namespace std;

static int fd;

void perror_exit(const char* str){
    perror(str);
    exit(EXIT_FAILURE);
}

void create_udp(const char *dst, const char *port){
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd==-1){
        perror_exit("socket");
    }
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    if(int ret=inet_pton(AF_INET,dst,(void*)&addr.sin_addr.s_addr); ret<=0){
        if(ret == 0){
            cerr << "Not a address" << endl;
            exit(EXIT_FAILURE);
        }
        else{
            perror_exit("inet_pton");
        }
    }
    if(connect(fd,(sockaddr*)&addr,sizeof(addr))!=0){
        perror_exit("connect");
    }
}

void send_garbage(){
    static char buf[4];
    send(fd,buf,sizeof(buf),0);
}

int main (int argc, char *argv[]){
    if(argc!=4){
        cerr << "Usage: trans host port data";
        exit(EXIT_FAILURE);
    }
    create_udp(argv[1],argv[2]);
    vector<int> data;
    for(char c:string(argv[3])){
        for(int i = 0 ; i < 8 ; i ++){
            data.push_back(c&1);
            c>>=1;
        }
    }
    for(auto& x:data){
        if(x==1)x=ONE;
        else x=ZERO;
    }
    for(auto x:data){
        send_garbage();
        usleep(x*1000);
    }
    send_garbage();
}
