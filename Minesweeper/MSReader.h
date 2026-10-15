#pragma once
#include <vector>
#include <windows.h>

struct ScreenCapture {
    int x0 = 0, y0 = 0;              // левый верхний угол области на экране
    int w = 0, h = 0;                // размеры области

    HDC     hdcScreen = nullptr;
    HDC     hdcMem = nullptr;
    HBITMAP hbm = nullptr;
    HBITMAP oldBitmap = nullptr;

    std::vector<BYTE> pixels;
    BITMAPINFO        bi = {};

    bool valid = false;      // успешно ли создан захват
};

struct RGB {
    int r, g, b;

    bool operator==(const RGB& other) const {
        return r == other.r && g == other.g && b == other.b;
    }
};

struct Reader {
private:
    int Rh = 0, Rw = 0;
    int** matrix = nullptr;

    int RscreenH, RscreenW;
    int RsizeBox;
    int R_x, R_y;
    
    ScreenCapture _CaptureScreen(int x, int y, int w, int h);
    RGB getColors(const ScreenCapture& cap, int field_x, int field_y);
    RGB getColors2(const ScreenCapture& cap, int field_x, int field_y);
    COLORREF _GetPixelColor(const ScreenCapture& cap, int px, int py);
    void _ReleaseCapture(ScreenCapture& cap);

    int fundXLH(const ScreenCapture& cap);
    int fundYLH(const ScreenCapture& cap, int _x);
    void fundRL(const ScreenCapture& cap, int _xlh, int _ylh, int* _xrl, int* _yrl);
    void fundRC(const ScreenCapture& cap, int _xlh, int _ylh, int _xrl, int _yrl, int& _r, int& _c);
public:
    Reader(int sH, int sW) : RscreenH(sH), RscreenW(sW){}

    void create();
    int init(int& sB, int& xX, int& yY, int& rrow, int& ccol);
    void set(int& sB, int& xX, int& yY, int& rrow, int& ccol);

    //void get_size(int& h, int& w);
    //void get_size2(int& h, int& w);
    int** readScreen();
    int** readScreen2();
    bool checkW();
    void free();
};

