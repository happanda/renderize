#pragma once


template <class Derived>
struct Singleton
{
    template <class ...Params>
    static void create(Params&&... params)
    {
        if (!sInstance)
            sInstance = new Derived(std::forward<Params>(params)...);
    }

    static void destroy()
    {
        if (sInstance)
        {
            delete sInstance;
            sInstance = nullptr;
        }
    }

    static Derived& inst()
    {
        return *sInstance;
    }

protected:
    Singleton()
    {
    }

private:
    static Derived* sInstance;
};

template <class Derived>
Derived* Singleton<Derived>::sInstance = nullptr;

#define FRIEND_SINGLETON(Derived) friend struct Singleton<Derived>;

#define INST_GET(Type, NAME) inline Type& NAME() { return Type::inst(); }