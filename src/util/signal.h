#pragma once
#include <nod.hpp>


template <class T>
using signal = nod::unsafe_signal<T>;

using connection = nod::scoped_connection;
