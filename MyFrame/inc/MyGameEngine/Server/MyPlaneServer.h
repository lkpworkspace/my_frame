#ifndef MyPlaneServer_H
#define MyPlaneServer_H

class MyPlaneServer : public MyPlane
{
public:
    static MyGameObj* StaticCreate() { return MyNetworkManagerServer::sInstance->RegisterGameObject( new MyPlaneServer() ); }

    virtual void Update() override;
protected:
    MyPlaneServer(){}
};

#endif
