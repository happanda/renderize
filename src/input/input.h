#pragma once


enum class KeyAction : char
{
    Pressed,
    Released,
    Repeated
};

enum KeyModifier : char
{
    modShift   = 1 >> 0,
    modControl = 1 >> 1,
    modAlt     = 1 >> 2,
    modSuper   = 1 >> 3
};
