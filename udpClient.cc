#include<iostream>
#include<string>
#include<cstring>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<arpa/inet.h>
void Usage(std::string proc)
{

    std::cerr<<"Usage"<<proc<<"SERVER_IP SERVER_PORT"<<std::endl;
    std::cerr<<"Version:1.0"<<std::endl;
}
//udpSe0rver ip port
int main(int argc,char* argv[])
{
    if(argc != 3){
        Usage(argv[0]);//手册
        return 1;
    }
    int sock = socket(AF_INET,SOCK_DGRAM,0);
    if(sock<0){
        std::cerr<<"socket error"<<std::endl;//cerr<< 标准错误输出
        return 2;
    }

    std::cout<<"sock"<<sock<<std::endl;
    
    //绑定
    struct sockaddr_in server;
    server.sin_family =AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]);
     
    //服务器---常驻进程 死循环
    char buf[1024];
    for(;;){
        std::cout<<"Please Enter# ";
        std::cin>>buf;
        sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&server,sizeof(server));
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        ssize_t s=recvfrom(sock,buf,sizeof(buf)-1,0,(struct sockaddr*)&peer,&len);
        if(s>0){
            buf[s] = 0;//给结尾加上\0;
            std::cout<<"server echo# "<<buf<<std::endl;
        }
    }

    close(sock);
    return 0;

}
