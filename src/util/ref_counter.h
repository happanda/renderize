#pragma once
#include <cstdint>


struct RefCounted
{
protected:
    mutable std::uint32_t* mRefCounter;

    RefCounted()
        : mRefCounter(new std::uint32_t)
    {
        *mRefCounter = 1;
    }

    RefCounted(RefCounted const& rhs)
        : mRefCounter(rhs.mRefCounter)
    {
        ++(*mRefCounter);
    }

    RefCounted const& operator=(RefCounted const& rhs)
    {
        decrementCounter();
        mRefCounter = rhs.mRefCounter;
        ++(*mRefCounter);
        return *this;
    }

    virtual ~RefCounted()
    {
        decrementCounter();
    }

    bool lastInstance() const
    {
        return *mRefCounter == 1;
    }

private:
    void decrementCounter()
    {
        if (lastInstance())
            delete mRefCounter;
        else
            --(*mRefCounter);
    }
};
