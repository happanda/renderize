#include <iostream>
#include "app/app.h"
#include "cubes.h"


int main(int argc, char* argv[])
{
    app::create();
    App().init();
    App().runFragmentDemo("clock");
    app::destroy();
}
