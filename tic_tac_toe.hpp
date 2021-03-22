#pragma once
#include <vector>

namespace tic_tac_toe
{
    enum class FieldState
    {
        NONE,
        FIRST,
        SECOND
    };

    static const int SIZE = 5;
    static const int continuous = 4;

    extern int is_sinish(std::vector<std::vector<FieldState>> field);
    extern FieldState get_next_turn(std::vector<std::vector<FieldState>> field);
    extern void print_field(std::vector<std::vector<FieldState>> field);
    extern bool is_in_field(int x, int y);
    extern int count_NONE(std::vector<std::vector<tic_tac_toe::FieldState>> field);
};