#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

struct Field {
private:
    int h = 0, w = 0;
    int** matrix = nullptr;
    int** matrix_bomb = nullptr;
    int** matrix_action = nullptr;

    void ma_clear();
    int VoClose(int _y, int _x);
    int VoBomb(int _y, int _x);
    void check_ab(int _y, int _x);
    int open_nb(int _y, int _x);
    void open_extra();
    bool is_Aclose();

    ///

public:
    bool check();
    void set(int _h, int _w);
    void create();
    void free();

    void input_matrix(int** matrix);
    int** output_matrix();

    void action();
    //int** get_action() const;
};