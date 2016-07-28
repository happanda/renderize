#pragma once
#include <cstdint>


struct RefCounted
{
    size_t hash() const;

protected:
    mutable std::uint32_t* mRefCounter;

    RefCounted();
    virtual ~RefCounted();
    RefCounted(RefCounted const& rhs);
    RefCounted const& operator=(RefCounted const& rhs);

    bool lastInstance() const;

private:
    void decrementCounter();
};
