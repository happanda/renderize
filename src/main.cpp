#include <iostream>
#include "app/app.h"
#include "cubes.h"


int main(int argc, char* argv[])
{
    App::create();
    APP().init();
    //APP().run();
    APP().runFragmentDemo("clock");
    App::destroy();
}
