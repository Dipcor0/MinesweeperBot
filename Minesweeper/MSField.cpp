#include "MSField.h"
#include <iostream>

bool Field::check() {
    bool res = true;
    for (int i = 0; i < h && res == true; ++i) {
        for (int j = 0; j < w && res == true; ++j) {
            if (matrix[i][j] == -2) {
                res = false;
            }
        }
    }
    return res;
}
bool Field::is_Aclose() {
    bool res = true;
    for (int i = 0; i < h && res == true; ++i) {
        for (int j = 0; j < w && res == true; ++j) {
            if (matrix[i][j] != -1) {
                res = false;
            }
        }
    }
    return res;

}
void Field::set(int _h, int _w) {
    h = _h;
    w = _w;
}

void Field::create() {
    if (h <= 0 || w <= 0) return;
    matrix = new int* [h];
    matrix_bomb = new int* [h];
    matrix_action = new int* [h];
    for (int i = 0; i < h; ++i) {
        matrix[i] = new int[w]();
        matrix_bomb[i] = new int[w]();
        matrix_action[i] = new int[w]();
    }
}

void Field::free() {
    for (int i = 0; i < h; ++i) {
        delete[] matrix[i];
        delete[] matrix_bomb[i];
        delete[] matrix_action[i];
    }
    delete[] matrix;
    delete[] matrix_bomb;
    delete[] matrix_action;

    matrix = nullptr;
    matrix_bomb = nullptr;
    matrix_action = nullptr;
    h = w = 0;
}

void Field::input_matrix(int** _matrix) {
    printf("get matrix\n");
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            matrix[i][j] = _matrix[i][j];
        }
    }

    //for (int i = 0; i < h; ++i) {
    //    for (int j = 0; j < w; ++j) {
    //        printf("%d\t", matrix[i][j]);
    //    }
    //    printf("\n");
    //}
}
int** Field::output_matrix() {
    printf("give action\n");
    
    return matrix_action;
}

void Field::ma_clear() {
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            matrix_action[i][j] = 0;
        }
    }
}
int Field::VoClose(int _y, int _x) {
    int counter = 0;
    for (int shift_x = -1; shift_x <= 1; shift_x++) {
        for (int shift_y = -1; shift_y <= 1; shift_y++) {
            if (0 <= _y + shift_y && _y + shift_y < h &&
                0 <= _x + shift_x && _x + shift_x < w &&
                !(shift_x == 0 && shift_y == 0) && 
                matrix[_y + shift_y][_x + shift_x] == -1) {
                counter++;
            }
        }
    }
    return counter;
}
int Field::VoBomb(int _y, int _x) {
    int counter = 0;
    for (int shift_x = -1; shift_x <= 1; shift_x++) {
        for (int shift_y = -1; shift_y <= 1; shift_y++) {
            if (0 <= _y + shift_y && _y + shift_y < h &&
                0 <= _x + shift_x && _x + shift_x < w &&
                !(shift_x == 0 && shift_y == 0) &&
                matrix_bomb[_y + shift_y][_x + shift_x] == 1) {
                counter++;
            }
        }
    }
    return counter;
}
void Field::check_ab(int _y, int _x) {
    for (int shift_x = -1; shift_x <= 1; shift_x++) {
        for (int shift_y = -1; shift_y <= 1; shift_y++) {
            if (0 <= _y + shift_y && _y + shift_y < h &&
                0 <= _x + shift_x && _x + shift_x < w &&
                !(shift_x == 0 && shift_y == 0) &&
                matrix[_y + shift_y][_x + shift_x] == -1) {
                matrix_bomb[_y + shift_y][_x + shift_x] = 1;
            }
        }
    }
}
int Field::open_nb(int _y, int _x) {
    int status = 1;
    for (int shift_x = -1; shift_x <= 1; shift_x++) {
        for (int shift_y = -1; shift_y <= 1; shift_y++) {
            if (0 <= _y + shift_y && _y + shift_y < h &&
                0 <= _x + shift_x && _x + shift_x < w &&
                !(shift_x == 0 && shift_y == 0) &&
                matrix_bomb[_y + shift_y][_x + shift_x] != 1 &&
                matrix[_y + shift_y][_x + shift_x] == -1) {

                matrix_action[_y + shift_y][_x + shift_x] = 1;
                status = 0;
            }
        }
    }
    return status;
}
//void Field::open_extra() {
//    int st = 0;
//    for (int i = 0; i < h && st == 0; ++i) {
//        for (int j = 0; j < w && st == 0; ++j) {
//            if (matrix[i][j] == -1 && matrix_bomb[i][j] != 1) {
//                matrix_action[i][j] = 1;
//                st = 1;
//            }
//        }
//    }
//}
void Field::action() {
    //for (int i = 0; i < h; ++i) {
    //    for (int j = 0; j < w; ++j) {
    //        printf("%d\t", VoCC(i, j));
    //    }
    //    printf("\n");
    //}
    //printf("\n");
    //for (int i = 0; i < h; ++i) {
    //    for (int j = 0; j < w; ++j) {
    //        printf("%d\t", VoCC(i, j) == matrix[i][j] && matrix[i][j] != 0);
    //    }
    //    printf("\n");
    //}
    printf("make action\n");
    ma_clear();
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (VoClose(i, j) == matrix[i][j]) {
                check_ab(i, j);
            }
        }
    }

    int status = 1;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (VoBomb(i, j) == matrix[i][j]) {
                status *= open_nb(i, j);
            }
        }
    }
    //printf("status %d\n", status);
    if (is_Aclose()) {
        matrix_action[h / 2][w / 2] = 1;
        status = 1;
    }

    if (status != 0) {
        open_extra();
    }

    //for (int i = 0; i < h; ++i) {
    //    for (int j = 0; j < w; ++j) {
    //        printf("%d\t", matrix_action[i][j] + 2 * matrix_bomb[i][j]);
    //    }
    //    printf("\n");
    //}
    //printf("mat\n");
    //for (int i = 0; i < h; ++i) {
    //    for (int j = 0; j < w; ++j) {
    //        printf("%d\t", matrix[i][j]);
    //    }
    //    printf("\n");
    //}
    //printf("bomb\n");
    //for (int i = 0; i < h; ++i) {
    //    for (int j = 0; j < w; ++j) {
    //        printf("%d\t", matrix_bomb[i][j]);
    //    }
    //    printf("\n");
    //}
    //printf("act\n");
    //for (int i = 0; i < h; ++i) {
    //    for (int j = 0; j < w; ++j) {
    //        printf("%d\t", matrix_action[i][j]);
    //    }
    //    printf("\n");
    //}
    printf("\n");
}

////

void Field::open_extra() {
    // 1. Собираем все неизвестные клетки (закрытые и не помеченные как бомбы)
    std::vector<std::pair<int, int>> unknownCells;
    std::vector<std::vector<int>> id(h, std::vector<int>(w, -1));
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (matrix[i][j] == -1 && matrix_bomb[i][j] != 1) {
                id[i][j] = unknownCells.size();
                unknownCells.push_back({ i, j });
            }
        }
    }
    int N = unknownCells.size();
    if (N == 0) return;

    // 2. Строим уравнения: для каждой открытой клетки с числом >= 0
    struct Equation {
        std::vector<int> vars; // индексы неизвестных клеток
        int rhs;               // сколько бомб среди них
    };
    std::vector<Equation> equations;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (matrix[i][j] >= 0) {
                std::vector<int> vars;
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (dx == 0 && dy == 0) continue;
                        int ny = i + dy, nx = j + dx;
                        if (ny < 0 || ny >= h || nx < 0 || nx >= w) continue;
                        if (matrix[ny][nx] == -1 && matrix_bomb[ny][nx] != 1) {
                            vars.push_back(id[ny][nx]);
                        }
                    }
                }
                if (!vars.empty()) {
                    int knownBombs = VoBomb(i, j);
                    int rhs = matrix[i][j] - knownBombs;
                    if (rhs >= 0 && rhs <= (int)vars.size()) {
                        equations.push_back({ vars, rhs });
                    }
                }
            }
        }
    }

    // 3. Оставляем только переменные, которые участвуют в уравнениях
    std::vector<bool> active(N, false);
    for (auto& eq : equations) {
        for (int v : eq.vars) active[v] = true;
    }
    std::vector<int> origIndex;
    for (int i = 0; i < N; ++i) {
        if (active[i]) origIndex.push_back(i);
    }
    int M = origIndex.size();
    if (M == 0) {
        // Нет уравнений — просто открываем первую попавшуюся
        for (auto& p : unknownCells) {
            matrix_action[p.first][p.second] = 1;
            return;
        }
    }

    // Перенумеровываем переменные для активных
    std::vector<int> newIndex(N, -1);
    for (int i = 0; i < M; ++i) newIndex[origIndex[i]] = i;

    // Преобразуем уравнения
    struct Eq {
        std::vector<int> vars;
        int rhs;
    };
    std::vector<Eq> eqs;
    for (auto& eq : equations) {
        Eq e;
        e.rhs = eq.rhs;
        for (int v : eq.vars) {
            e.vars.push_back(newIndex[v]);
        }
        if (!e.vars.empty()) eqs.push_back(e);
    }

    // 4. CSP-решатель: проверяет, существует ли решение при заданных значениях
    std::function<bool(std::vector<int>&)> solve = [&](std::vector<int>& val) -> bool {
        // Пропагация ограничений
        bool changed = true;
        while (changed) {
            changed = false;
            for (auto& eq : eqs) {
                int sum = 0;
                int unassigned = 0;
                for (int v : eq.vars) {
                    if (val[v] == 1) sum++;
                    else if (val[v] == -1) unassigned++;
                }
                if (sum > eq.rhs) return false;
                if (sum + unassigned < eq.rhs) return false;
                if (unassigned == 0) {
                    if (sum != eq.rhs) return false;
                }
                else {
                    if (sum == eq.rhs) {
                        for (int v : eq.vars) {
                            if (val[v] == -1) {
                                val[v] = 0;
                                changed = true;
                            }
                        }
                    }
                    else if (sum + unassigned == eq.rhs) {
                        for (int v : eq.vars) {
                            if (val[v] == -1) {
                                val[v] = 1;
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
        // Ищем нераспределённую переменную
        int next = -1;
        for (int i = 0; i < M; ++i) {
            if (val[i] == -1) {
                next = i;
                break;
            }
        }
        if (next == -1) return true; // все распределены — решение найдено
        // Пробуем 0
        val[next] = 0;
        if (solve(val)) return true;
        val[next] = -1;
        // Пробуем 1
        val[next] = 1;
        if (solve(val)) return true;
        val[next] = -1;
        return false;
        };

    // 5. Ищем клетку, которая обязана быть безопасной (0).
    // Для этого предполагаем, что она бомба (1). Если решения нет — она безопасна.
    int safeActive = -1;
    for (int i = 0; i < M; ++i) {
        std::vector<int> val(M, -1);
        val[i] = 1; // предполагаем бомбу
        if (!solve(val)) {
            safeActive = i;
            break;
        }
    }

    if (safeActive != -1) {
        int orig = origIndex[safeActive];
        auto p = unknownCells[orig];
        matrix_action[p.first][p.second] = 1;
        return;
    }

    // 6. Если не нашли гарантированно безопасную клетку через уравнения,
    // открываем первую попавшуюся неизвестную клетку.
    for (auto& p : unknownCells) {
        matrix_action[p.first][p.second] = 1;
        return;
    }
}