#include "state.hpp"
#include <cstdio>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <iostream>

State::State()
{
    this->field.resize(tic_tac_toe::SIZE);
    for (int i = 0; i < tic_tac_toe::SIZE; ++i)
    {
        this->field[i].resize(tic_tac_toe::SIZE);
        for (int j = 0; j < tic_tac_toe::SIZE; ++j)
        {
            this->field[i][j] = tic_tac_toe::FieldState::NONE;
        }
    }

    this->sum = 0;
    this->win = 0;
    this->children.resize(0);
    this->score.resize(0);
}

State::State(std::vector<std::vector<tic_tac_toe::FieldState>> field)
{
    this->field.resize(tic_tac_toe::SIZE);
    for (int i = 0; i < tic_tac_toe::SIZE; ++i)
    {
        this->field[i].resize(tic_tac_toe::SIZE);
        for (int j = 0; j < tic_tac_toe::SIZE; ++j)
        {
            this->field[i][j] = field[i][j];
        }
    }

    this->sum = 0;
    this->win = 0;
    this->children.resize(0);
    this->score.resize(0);
}

void State::find_children()
{
    tic_tac_toe::FieldState next_turn = tic_tac_toe::get_next_turn(this->field);

    int idx = 0;
    for (int y = 0; y < tic_tac_toe::SIZE; ++y)
    {
        for (int x = 0; x < tic_tac_toe::SIZE; ++x)
        {
            if (this->field[y][x] == tic_tac_toe::FieldState::NONE)
            {
                std::vector<std::vector<tic_tac_toe::FieldState>> next_field = this->field;
                next_field[y][x] = next_turn;
                State child(next_field);

                this->children.push_back(child);
                this->score.push_back(0);
            }
        }
    }
}

void State::set_ucb1()
{
    for (int i = 0; i < this->children.size(); ++i)
    {
        if (this->children[i].sum == 0)
        {
            this->score[i] = 10000;
            continue;
        }
        double win_rate = (double)this->children[i].win / this->children[i].sum;
        double u = std::sqrt(2.0 * std::log(this->sum) / this->children[i].sum);

        this->score[i] = win_rate + u;
    }
}

void State::add(int index, int value)
{
    this->sum += 1;
    this->children[index].sum += 1;
    this->children[index].win += value;
}

void State::add(int value)
{
    this->sum += 1;
    this->win += value;
}

void State::print_sum()
{
    puts("--sum----------------------------------");
    int idx = 0;
    for (int y = 0; y < tic_tac_toe::SIZE; y++)
    {
        for (int x = 0; x < tic_tac_toe::SIZE; x++)
        {
            if (this->field[y][x] == tic_tac_toe::FieldState::NONE)
            {
                printf("%5d ", this->children[idx++].sum);
            }
            else
            {
                printf("    - ", 0);
            }
        }
        putchar('\n');
    }
}

void State::print_win()
{
    puts("--win----------------------------------");
    int idx = 0;
    for (int y = 0; y < tic_tac_toe::SIZE; y++)
    {
        for (int x = 0; x < tic_tac_toe::SIZE; x++)
        {
            if (this->field[y][x] == tic_tac_toe::FieldState::NONE)
            {
                printf("%5d ", this->children[idx++].win);
            }
            else
            {
                printf("    - ", 0);
            }
        }
        putchar('\n');
    }
}

void State::print_result()
{
    std::vector<double> result = State::get_result();

    puts("--ans----------------------------------");
    int idx = 0;
    for (int y = 0; y < tic_tac_toe::SIZE; ++y)
    {
        for (int x = 0; x < tic_tac_toe::SIZE; ++x)
        {
            if (field[y][x] == tic_tac_toe::FieldState::NONE)
            {
                printf("%+.3f ", result[idx++]);
            }
            else
            {
                printf("     - ");
            }
        }
        putchar('\n');
    }
}

int State::play_out()
{
    // ‚Í‚¶‚ß‚Ä‚È‚ç
    if (this->children.size() == 0)
    {
        State::find_children();
    }

    // UCB1‚ÌŒvŽZ
    State::set_ucb1();

    // ŽŸ‚ÌŽè‚ðŒˆ‚ß‚é
    double max_ucb1 = -1.0;
    int max_index = 0;
    for (int i = 0; i < this->score.size(); ++i)
    {
        if (this->score[i] > max_ucb1)
        {
            max_ucb1 = this->score[i];
            max_index = i;
        }
    }

    std::vector<std::vector<tic_tac_toe::FieldState>> virtual_field = this->children[max_index].field;

    int point = 0;

    while ((point = tic_tac_toe::is_sinish(virtual_field)) == -2)
    {
        tic_tac_toe::FieldState now = tic_tac_toe::get_next_turn(virtual_field);

        int x, y;
        do
        {
            x = rand() % tic_tac_toe::SIZE;
            y = rand() % tic_tac_toe::SIZE;
        } while (virtual_field[y][x] != tic_tac_toe::FieldState::NONE);

        virtual_field[y][x] = now;
    }

    State::add(max_index, point);
    return point;
}

std::vector<double> State::get_result()
{
    std::vector<double> win_rate(this->children.size());
    for (int i = 0; i < this->children.size(); ++i)
    {
        if (this->children[i].sum == 0)
        {
            win_rate[i] = 0.0;
            continue;
        }
        win_rate[i] = (double)this->children[i].win / this->children[i].sum;
    }

    return win_rate;
}