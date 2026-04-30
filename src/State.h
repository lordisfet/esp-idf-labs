#pragma once

enum class State
{
    OFF = 0,
    ON = 1
};

inline State operator!(State s)
{
    return (s == State::OFF) ? State::ON : State::OFF;
}
