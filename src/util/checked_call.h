#pragma once


#define CHECK(expr, msg, action)        \
do{                                     \
    if (!(expr))                        \
    {                                   \
        std::cerr << msg << std::endl;  \
        do {                            \
            action;                     \
        } while(false);                 \
    }                                   \
} while(false);