#include <iostream>
#include "MyFrame.h"

using namespace std;
using namespace my_master;
int main(int argc, char *argv[])
{
    argc = argc;
    argv = argv;

    MyApp app(4,1024);
    cout << "Hello World!" << endl;
    return app.Exec();
}
