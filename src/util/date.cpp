#include <chrono>
#include <iostream>
#include "date.h"


using namespace std;
using namespace std::chrono;

typedef duration<int, ratio<3600 * 24>>  DaysDuration;


float secFrom00()
{
    auto now = high_resolution_clock::now();
    auto tmt = high_resolution_clock::to_time_t(now);
    auto tm = localtime(&tmt);

    auto tp = now.time_since_epoch();
    DaysDuration days = duration_cast<DaysDuration>(tp);
    tp -= days;
    hours h = duration_cast<hours>(tp);
    tp -= h;
    minutes m = duration_cast<minutes>(tp);
    tp -= m;
    seconds s = duration_cast<seconds>(tp);
    tp -= s;

    auto ms = duration_cast<milliseconds>(tp);
    return static_cast<float>(((tm->tm_hour * 60) + tm->tm_min) * 60 + tm->tm_sec)
        + static_cast<float>(ms.count()) / 1000.0f;
}
