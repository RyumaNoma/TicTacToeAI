#include "tic_tac_toe.hpp"
#include <cstdio>

int tic_tac_toe::is_sinish(std::vector<std::vector<tic_tac_toe::FieldState>> field)
{
    int dx[] = {1, 1, 0, -1};
    int dy[] = {0, 1, 1, 1};

    bool is_line;
    // ç¿ïW
    for (int y = 0; y < tic_tac_toe::SIZE; ++y)
    {
        for (int x = 0; x < tic_tac_toe::SIZE; ++x)
        {
            if (field[y][x] == tic_tac_toe::FieldState::NONE) continue;

            // ï˚å¸
            for (int i = 0; i < 4; i++)
            {
                tic_tac_toe::FieldState target = field[y][x];
                int nextX = x;
                int nextY = y;

                // òAë±
                is_line = true;
                for (int last = 0; last < tic_tac_toe::continuous; ++last)
                {
                    if (tic_tac_toe::is_in_field(nextX, nextY))
                    {
                        if (field[nextY][nextX] == target)
                        {
                            nextX += dx[i];
                            nextY += dy[i];
                        }
                        else
                        {
                            is_line = false;
                            break;
                        }
                    }
                    else
                    {
                        is_line = false;
                        break;
                    }
                }

                if (is_line)
                {
                    switch (target)
                    {
                    case tic_tac_toe::FieldState::FIRST:
                        return 1;
                    case tic_tac_toe::FieldState::SECOND:
                        return -1;
                    default:
                        break;
                    }
                }
            }
        }
    }

    if (tic_tac_toe::count_NONE(field) == 0) return 0;

    return -2;
}

tic_tac_toe::FieldState tic_tac_toe::get_next_turn(std::vector<std::vector<tic_tac_toe::FieldState>> field)
{
    int first = 0;
    int second = 0;

    for (int y = 0; y < tic_tac_toe::SIZE; ++y)
    {
        for (int x = 0; x < tic_tac_toe::SIZE; ++x)
        {
            switch (field[y][x])
            {
            case tic_tac_toe::FieldState::FIRST:
                ++first;
                break;
            case tic_tac_toe::FieldState::SECOND:
                ++second;
                break;
            }
        }
    }

    return ((first > second) ? tic_tac_toe::FieldState::SECOND : tic_tac_toe::FieldState::FIRST);
}

void tic_tac_toe::print_field(std::vector<std::vector<tic_tac_toe::FieldState>> field)
{
    puts("------------------------");
    for (int y = 0; y < tic_tac_toe::SIZE; ++y)
    {
        for (int x = 0; x < tic_tac_toe::SIZE; ++x)
        {
            printf("%d ", field[y][x]);
        }
        putchar('\n');
    }
    puts("------------------------");
}

bool tic_tac_toe::is_in_field(int x, int y)
{
    if (x < 0 || tic_tac_toe::SIZE <= x) return false;
    if (y < 0 || tic_tac_toe::SIZE <= y) return false;

    return true;
}

int tic_tac_toe::count_NONE(std::vector<std::vector<tic_tac_toe::FieldState>> field)
{
    int count = 0;
    for (int y = 0; y < tic_tac_toe::SIZE; ++y)
    {
        for (int x = 0; x < tic_tac_toe::SIZE; ++x)
        {
            if (field[y][x] == tic_tac_toe::FieldState::NONE) ++count;
        }
    }

    return count;
}