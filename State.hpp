#pragma once
#include <vector>
#include <cmath>
#include "tic_tac_toe.hpp"

class State
{
private:
    std::vector<std::vector<tic_tac_toe::FieldState>> field;
    int sum;
    int win;
    std::vector<State> children;
    std::vector<double> score;
public:
    State();
    State(std::vector<std::vector<tic_tac_toe::FieldState>> field);

    void find_children();
    void set_ucb1();
    void add(int index, int value);
    void add(int value);
    void print_sum();
    void print_win();
    void print_result();
    int play_out();
    std::vector<double> get_result();
};