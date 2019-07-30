
#include<iostream>
#include<unistd.h>
#include<stdio.h>
#include<sys/select.h>

//int select(int nfds,fd_set *readfds,fd_set *writefds,fd_set *exceptfds,struct timeval *timeout)
int main()
{
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(0,&read_fds);

    for(;;)
    {
        std::cout<<"> "<<std::endl;
        fflush(stdout);
        int ret = select(1,&read_fds,nullptr,nullptr,nullptr);
        if(ret<0)
        {
            std::cout<<"select error"<<std::endl;
            continue;
        }
        else if(0 == ret)
        {
        std::cout<<"timeout..."<<std::endl;
            continue;
        }
        else
        {
            char buf[1024]={0};
            read(0,buf,sizeof(buf)-1);
            std::cout<<"input:"<<buf<<std::endl;
        }
    }

    FD_ZERO(&read_fds);
    //FD_SET(0,&read_fds);

    return 0;

}
