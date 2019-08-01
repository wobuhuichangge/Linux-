#include"IMserver.hpp"

using namespace std;
int main()
{
    
    IMServer *is =  new IMServer() ;
    is->InitServer();
    is->Start();

    return 0;
}
