#pragma once
#include <vector>


template <class Iter, class Func>
void cycle2(Iter first, Iter last, Func func)
{
    for (Iter iter = first; iter != last; ++iter)
    {
        Iter next = iter;
        ++next;
        if (next == last)
            next = first;
        func(*iter, *next);
    }
}
