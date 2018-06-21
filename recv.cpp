#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include "protocol.hpp"

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
    size_t time;
    timer(){
        gettimeofday(&s,NULL);
    }
    void stop(){
        gettimeofday(&e,NULL);
        size_t sec = e.tv_sec - s.tv_sec;
        size_t usec = e.tv_usec - s.tv_usec;
        time=sec*1000000 + usec;
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

vector<bool> to_data(const vector<size_t>& time){
    vector<bool> ret;
    size_t x = 1;
    for(size_t i = 0 ; i < time.size() ; i ++){
        if(i+1==x){
            x<<=1;
            if(time[i]<D[2]){
                ret.push_back(0);
            }
            else{
                ret.push_back(1);
            }
        }
        else{
            if(time[i]<D[1]){
                ret.push_back(0);
            }
            else{
                ret.push_back(1);
            }
        }
    }
    return ret;
}

vector<bool> decode(vector<bool> data){
    for(auto x:data){
        printf("%d,",(int)x);
    }
    puts("");
    bool error=0;
    size_t pos=0;
    for(size_t x=1 ; x-1<data.size() ; x<<=1){
        bool par=0;
        for(size_t i = 0 ; i < data.size() ; i ++){
            if((i+1)&x){
                par^=data[i];
            }
        }
        if(par){
            error=true;
            pos|=x;
        }
    }
    if(error){
        cerr << "!! Have error, Fixing it" << endl;
        data[pos-1]=!data[pos-1];
    }
    vector<bool> ret;
    size_t x= 1;
    for(size_t i = 0 ; i < data.size() ; i ++){
        if(i+1==x){
            x*=2;
        }
        else{
            ret.push_back(data[i]);
        }
    }
    return ret;
}

size_t to_int(vector<bool> data){
    size_t x=1;
    size_t res=0;
    for(auto v:data){
        res+=x*v;
        x<<=1;
    }
    return res;
}

string to_str(vector<bool> data){
    int x=1;
    char c=0;
    string res;
    for(auto v:data){
        c+=x*v;
        if(x==1<<7){
            res+=c;
            x=1;
            c=0;
        }
        else{
            x<<=1;
        }
    }
    return res;
}

char get_char(){
    static char buf[100];
    vector<size_t> time;
    for(int i = 0 ; i < 12 ; i ++){
        timer t;
        recv(fd,buf,sizeof(buf),0);
        t.stop();
        if(t.time>=D[0]){
            time.push_back(t.time);
            cout << i << ": ";
            t.print();
        }
        else i--;
    }
    char c=to_int(decode(to_data(time)));
    return c;
}


int main (int argc, char *argv[]){
    if(argc!=2){
        cerr << "Usage: recv port";
        exit(EXIT_FAILURE);
    }
    create_udp(argv[1]);
    char buf[100];
    recv(fd,buf,sizeof(buf),0);
    size_t cnt = get_char();
    cout << "Data cnt: " << cnt << endl;
    string out;
    for(size_t i = 0 ; i < cnt ; i ++){
        out+=get_char();
    }
    cout << "Data: "  << out  << endl;
}
