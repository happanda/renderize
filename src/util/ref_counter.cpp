#include "ref_counter.h"


RefCounted::RefCounted()
    : mRefCounter(new std::uint32_t)
{
    *mRefCounter = 1;
}

RefCounted::~RefCounted()
{
    decrementCounter();
}

RefCounted::RefCounted(RefCounted const& rhs)
    : mRefCounter(rhs.mRefCounter)
{
    ++(*mRefCounter);
}

RefCounted const& RefCounted::operator=(RefCounted const& rhs)
{
    decrementCounter();
    mRefCounter = rhs.mRefCounter;
    ++(*mRefCounter);
    return *this;
}

bool RefCounted::lastInstance() const
{
    return *mRefCounter == 1;
}

void RefCounted::decrementCounter()
{
    if (lastInstance())
        delete mRefCounter;
    else
        --(*mRefCounter);
}
