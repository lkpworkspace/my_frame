#include "Server/MyServerShared.h"


int main()
{
    if( MyGameServer::StaticInit() )
    {
        return MyGameServer::sInstance->Go();
    }
    else
    {
        //error
        return -1;
    }
}
