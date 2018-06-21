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
const int BURST_CNT=5;

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
    for(int i = 0 ; i < BURST_CNT ; i ++){
        send(fd,buf,sizeof(buf),0);
    }
}

vector<int> encode(vector<bool> data){
    vector<int> ret;
    size_t nextck=1;
    for(auto x:data){
        while(ret.size()+1 == nextck){
            ret.push_back(0);
            nextck<<=1;
        }
        ret.push_back(x);
    }
    while(1){
        nextck>>=1;
        bool par=false;
        for(size_t i = 0 ; i < ret.size() ; i ++){
            if((i+1)&nextck && ret[i]==1){
                par=!par;
            }
        }
        ret[nextck-1]=2+par;
        if(nextck==1)break;
    }
    return ret;
}

vector<int> to_time(vector<int> data){
    for(auto& x:data){
        x=I[x];
    }
    return data;
}

vector<int> from_int(size_t x){
    vector<bool> tmp;
    for(int i = 0 ; i < 11 ; i ++){
        tmp.push_back(x&1);
        x>>=1;
    }
    return to_time(encode(move(tmp)));
}

void send_data(const vector<int> &time){
    for(auto x:time){
        send_garbage();
        usleep(x);
    }
}

int main (int argc, char *argv[]){
    if(argc!=4){
        cerr << "Usage: trans host port data";
        exit(EXIT_FAILURE);
    }
    create_udp(argv[1],argv[2]);
    vector<bool> tmp;
    for(char c:string(argv[3])){
        for(int i = 0 ; i < 8 ; i ++){
            tmp.push_back(c&1);
            c>>=1;
        }
    }
    auto data=to_time(encode(tmp));
    auto header=from_int(data.size());
    send_data(header);
    send_data(data);
    send_garbage();
}
