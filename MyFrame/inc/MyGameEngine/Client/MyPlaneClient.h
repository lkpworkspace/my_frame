#ifndef MyPlaneClient_H
#define MyPlaneClient_H

class MyPlaneClient : public MyPlane
{
public:
    static	MyGameObj* StaticCreate() { return  (MyGameObj*)new MyPlaneClient(); }

    virtual void Update();
    //virtual void HandleDying() override;

    virtual void Read( MyInputStream& inInputStream ) override;

protected:
	MyPlaneClient();

};

#endif
