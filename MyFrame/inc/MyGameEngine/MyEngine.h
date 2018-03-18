#ifndef MyEngine_H
#define MyEngine_H

class MyEngine
{
public:
    virtual ~MyEngine();
    virtual int		Run();
protected:
    MyEngine();
    virtual void	DoFrame();
private:
};

#endif
