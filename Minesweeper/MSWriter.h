#pragma once
#include <windows.h>

struct Writer {
private:
	int Wh = 0, Ww = 0;
	int WscreenH, WscreenW;
	int WsizeBox;
	int W_x, W_y;

	void click();
public:
	Writer(int sH, int sW) :
		WscreenH(sH), WscreenW(sW) {}

	void set(int& sB, int& xX, int& yY, int& rrow, int& ccol);
	void write_action(int** matrix_action);
	void reload();
	void win();
};