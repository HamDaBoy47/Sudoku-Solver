#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <array>
#include <bitset>
#include <cmath>
#include <set>
#include <chrono>

using namespace std;

std::vector<std::vector<char>> board;
int size;

std::array<std::bitset<9>, 9> row_contains = {0, 0, 0, 0, 0, 0, 0, 0, 0};
std::array<std::bitset<9>, 9> col_contains = {0, 0, 0, 0, 0, 0, 0, 0, 0};
std::array<std::bitset<9>, 9> cell_contains = {0, 0, 0, 0, 0, 0, 0, 0, 0};

class User_input{
public:

string input_values(string input)
{
    for (int i = 0; i < input.length(); ++i)
    { 
        if(input[i] == ' ')
        {
            input.erase(i, 1);   
        }
    }

    return input;
}

int counter(string input)
{
    string new_input = input_values(input);

    int counter = 0;

    for(int i = 0; i < new_input.length(); ++i)
    {
        ++counter;
    }

    return counter;
}

string new_input(string input)
{   
    string new_input;
    int count;
    
    new_input = new_input + input;
    new_input = new_input + " ";

    new_input = input_values(new_input);
    count = counter(new_input);
    
    for (int i = 1; i < count; ++i)
    {
        getline(cin, input);
        new_input = new_input + input;
        new_input = new_input + " ";
    }

    new_input = input_values(new_input);

     for (int i = 0; i < new_input.length(); ++i)
    {
        int charvalue = new_input[i];
        if (charvalue >= 65)
        {
            new_input[i] = char(charvalue - 7);
        }
    }

    return new_input;
}
};

class Sudoku_board{
public:

void flat_board_to_vec_vec(string input)
{
    User_input tmp;
    int count = tmp.counter(input);
    size = sqrt(count);
    
    board.reserve(size);

    for(int row = 0; row < size; ++row)
    {
        std::vector<char> this_row;
        this_row.reserve(size);

        for(int col = 0; col < size; ++col)
        {
            this_row.push_back(input[(row * size) + col]);
        }
        board.push_back(this_row);
    }

}

void converter()
{
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            int charvalue = board[row][col];
            if (charvalue >= 58)
            {
                board[row][col] = char(charvalue + 7); 
            }
        }
    }
}

void print_board()
{
    for (int row = 0; row < size ; ++row)
    {
        cout << board[row][0];
        for (int col = 1; col < size; ++col)
        {
            cout << ' ' << board[row][col];
        }
        cout << endl;
    }
}

};


constexpr int get_cell(int row, int col) noexcept
{
    return (row / 3) * 3 + col / 3;         //NEED TO ACCOUNT FOR SIZE
}

constexpr int get_next_row(int row, int col) noexcept
{
    return row + (col + 1) / 9;             //NEED TO ACCOUNT FOR SIZE
}

constexpr int get_next_col(int col) noexcept
{
    return (col + 1) % 9;                   //NEED TO ACCOUNT FOR SIZE
}

int next_empty_row_position(int row, int col) noexcept
{
    int size = board.size();

    while (row != size)
    {
        if (board[row][col] == '0')
        {
            return row;
        }
        row = get_next_row(row, col);
        col = get_next_col(col);
    }

    return 9;   //NEED TO ACCOUNT FOR SIZE
}

int next_empty_col_position(int row, int col) noexcept
{
    int size = board.size();

    while (row != size)
    {
        if (board[row][col] == '0')
        {
            return col;
        }
        row = get_next_row(row, col);
        col = get_next_col(col);
    }

    return 0;
}


class Solution{
public:

void solveSudoku() const noexcept
{
   for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col  < size; ++col)
        {
            char digit;
            if ((digit = board[row][col]) != '0')
            {
                int digit_idx = digit - '1';
                row_contains[row].set(digit_idx);
                col_contains[col].set(digit_idx);
                int cell = get_cell(row, col);
                cell_contains[cell].set(digit_idx);
            }
        }
    }
    solve(0, 0);
}


static bool solve(int const row_start, int const col_start) noexcept
{
    int size = board.size(); 
    auto row = next_empty_row_position(row_start, col_start);
    auto col = next_empty_col_position(row_start, col_start);

    if (row == size)
    {
        return true;
    }

    int const cell = get_cell(row, col);
    std::bitset<9> const contains = row_contains[row] | col_contains[col] | cell_contains[cell];
    if (contains.all())
    {
        return false;
    }

    for (int digit_idx = 0; digit_idx < size; ++digit_idx)
    {
        if (!contains[digit_idx])
        {
            board[row][col] = static_cast<char>(digit_idx + '1');
            row_contains[row].set(digit_idx);
            col_contains[col].set(digit_idx);
            cell_contains[cell].set(digit_idx);
            if (solve(row, col))
            {
                return true;
            }

            row_contains[row].reset(digit_idx);
            col_contains[col].reset(digit_idx);
            cell_contains[cell].reset(digit_idx);
        }
    }
    board[row][col] = '0';
    return false;
}

};


int main()
{
    User_input final_input;
    Sudoku_board new_board;
    Solution soln;

    string line;
    string input;

    getline(cin, line);

    input = final_input.new_input(line);

    //auto start = chrono::high_resolution_clock::now(); // Start the timer

    new_board.flat_board_to_vec_vec(input);

    cout << endl;

    soln.solveSudoku();

    if (board[8][8] == '0')
    {
        cout << "No Solution" << endl;
    }
    else
    {
        new_board.converter();
        new_board.print_board();
    }

    //auto end = chrono::high_resolution_clock::now();                                                          // Used to time the solve
    //cout << "Duration " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl; // Displays time taken to solve

    return 0;
}