#include "MSWriter.h"
#include <iostream>

void Writer::set(int& sB, int& xX, int& yY, int& rrow, int& ccol) {
    Wh = rrow;
    Ww = ccol;

    W_x = xX;
    W_y = yY;
    WsizeBox = sB;
}
void Writer::click() {
    INPUT inputs[2] = {};

    // Нажатие левой кнопки
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    // Отпускание
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(2, inputs, sizeof(INPUT));
};

void Writer::write_action(int** matrix_action) {
    printf("do action\n");
    //printf("Rh %d Rw %d RsH %d RSW %d Rsize %d _x %d _y %d\n", Wh, Ww, WscreenH, WscreenW, WsizeBox, W_x, W_y);
    for (int i = 0; i < Wh; ++i) {
        for (int j = 0; j < Ww; ++j) {
            //printf("%d\t" , matrix_action[i][j]);
            if (matrix_action[i][j] == 1) {
                SetCursorPos(W_x + WsizeBox / 2 + j * WsizeBox, W_y + WsizeBox * 2 / 3 + i * WsizeBox);
                //printf("x %d y %d", WsizeBox / 2 + j * WsizeBox, WsizeBox * 2 / 3 + i * WsizeBox);
                click();
            }
        }
        //printf("\n");
    }
    SetCursorPos(W_x + Ww * WsizeBox, W_y + Wh * WsizeBox);
}

void Writer::reload() {
    SetCursorPos(W_x + WsizeBox * Ww / 2, W_y - WsizeBox * 2);
    //printf("x %d y %d", WsizeBox / 2 + j * WsizeBox, WsizeBox * 2 / 3 + i * WsizeBox);
    click();
    Sleep(35);
}
void Writer::win() {
    ///
    int X = 1072, Y = 224;
    ///

    SetCursorPos(X, Y);
    //click();
}