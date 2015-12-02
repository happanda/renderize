#include <iostream>
#include "app/app.h"
#include "cubes.h"
#include "fragment_demo.h"
#include "landscape.h"
#include "universe.h"
#include "visual.h"


int main(int argc, char* argv[])
{
    app::create();
    App().init();
    App().runFragmentDemo("clock");
    app::destroy();
}
