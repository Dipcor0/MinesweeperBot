#include "MSReader.h"
//old
//std::vector<RGB> colors = {
//        {0, 0,0},
//        { 76,  84,  92 },
//        { 56,  64,  72 },
//        {124, 199, 255 },
//        {102, 194, 102 },
//        {255, 119, 136 },
//        {238, 136, 255 },
//        {221, 170,  34 },
//        {102, 204, 204 },
//        {153, 153, 153 },
//        {208, 216, 224 }
//};
std::vector<RGB> colors = {
    {170, 0, 67},///
    {170, 170, 170},
    {126, 126, 126},
    {0, 0, 170},///
    {0, 82, 0},
    {170, 0,0},
    {0, 0, 82},
    {82, 0,0},
    {0, 82, 82},
    {0, 0,0},
    {82,82,82}
};
//
ScreenCapture Reader::_CaptureScreen(int x, int y, int w, int h) {
    ScreenCapture cap;
    cap.x0 = x;
    cap.y0 = y;
    cap.w = w;
    cap.h = h;

    if (w <= 0 || h <= 0) return cap;    // ничего не делаем

    cap.hdcScreen = GetDC(NULL);
    if (!cap.hdcScreen) return cap;

    cap.hdcMem = CreateCompatibleDC(cap.hdcScreen);
    cap.hbm = CreateCompatibleBitmap(cap.hdcScreen, w, h);
    cap.oldBitmap = (HBITMAP)SelectObject(cap.hdcMem, cap.hbm);

    // Копируем область экрана в память
    BitBlt(cap.hdcMem, 0, 0, w, h, cap.hdcScreen, x, y, SRCCOPY);

    // Описание формата: 32 бита, BGRA, сверху вниз
    cap.bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    cap.bi.bmiHeader.biWidth = w;
    cap.bi.bmiHeader.biHeight = -h;
    cap.bi.bmiHeader.biPlanes = 1;
    cap.bi.bmiHeader.biBitCount = 32;
    cap.bi.bmiHeader.biCompression = BI_RGB;

    // Буфер под пиксели
    cap.pixels.resize((size_t)w * h * 4);

    // Выгружаем пиксели снимка в буфер
    int lines = GetDIBits(cap.hdcMem, cap.hbm, 0, h,
        cap.pixels.data(), &cap.bi, DIB_RGB_COLORS);
    if (lines == 0) {
        _ReleaseCapture(cap);    // что-то пошло не так — убираем за собой
        return cap;
    }

    cap.valid = true;
    return cap;
}
//
RGB Reader::getColors(const ScreenCapture& cap, int field_x, int field_y) {
    RGB result = { 0, 0, 0 };
    //printf("Rh %d Rw %d RsH %d RSW %d Rsize %d _x %d _y %d\n", Rh, Rw, RscreenH, RscreenW, RsizeBox, R_x, R_y);

    //printf("fx %d fy %d\n", field_x, field_y);
    if (cap.valid) {
        // 2. Читаем пиксель — центр клетки
        COLORREF base = _GetPixelColor(cap, RsizeBox / 2 + field_x * RsizeBox, RsizeBox / 3 + field_y * RsizeBox);
        //COLORREF flor = _GetPixelColor(cap, RsizeBox / 5 + field_x * RsizeBox, RsizeBox * 5 + field_y * RsizeBox);
        //SetCursorPos(RsizeBox / 2 + R_x * RsizeBox, RsizeBox * 2 / 3 + R_y * RsizeBox);
        //printf("$fx %d $fy %d\n", RsizeBox / 2 + field_x * RsizeBox, RsizeBox / 4 + field_y * RsizeBox);
        if (base != CLR_INVALID) {
            result.r = GetRValue(base);
            result.g = GetGValue(base);
            result.b = GetBValue(base);
            printf("color: (%d, %d, %d)\n", result.r, result.g, result.b);
        }
        else {
            printf("ERRORCOLOR\n");
        }
    }
    return result;
}
RGB Reader::getColors2(const ScreenCapture& cap, int field_x, int field_y) {
    RGB result = { 0, 0, 0 };
    //printf("Rh %d Rw %d RsH %d RSW %d Rsize %d _x %d _y %d\n", Rh, Rw, RscreenH, RscreenW, RsizeBox, R_x, R_y);

    //printf("fx %d fy %d\n", field_x, field_y);
    if (cap.valid) {
        // 2. Читаем пиксель — центр клетки
        
        COLORREF p1 = _GetPixelColor(cap,  field_x * RsizeBox + 3, field_y * RsizeBox + 3);
        COLORREF p2 = _GetPixelColor(cap, RsizeBox / 2 + field_x * RsizeBox, RsizeBox / 2 + field_y * RsizeBox);
        COLORREF p3 = _GetPixelColor(cap, RsizeBox / 2 + field_x * RsizeBox, RsizeBox / 4 + field_y * RsizeBox);

        if (p1 != CLR_INVALID && p2 != CLR_INVALID && p3 != CLR_INVALID) {
            RGB c1 = { GetRValue(p1), GetGValue(p1), GetBValue(p1) };
            RGB c2 = { GetRValue(p2), GetGValue(p2), GetBValue(p2) };
            RGB c3 = { GetRValue(p3), GetGValue(p3), GetBValue(p3) };
            //printf("%d %d %d\n", c1.r, c1.g, c1.b);
            //printf("%d %d %d\n", c2.r, c2.g, c2.b);
            //printf("%d %d %d\n", c3.r, c3.g, c3.b);
            if (c1 == colors[1]) { // закрыто
                //printf("1\n");
                result = c1;
            }
            else if (c1.r == colors[0].r && c2.g == colors[0].g && c2.b == 0) { // мина
                //printf("2\n");
                c1.b = 67;
                result = c1;
            }
            else if(c1 == colors[2]) { // открыто
                //printf("3\n");
                if (c2 != colors[2]) {
                    result = c2;
                }
                else{
                    result = c3;
                }

            }
        }
        //COLORREF flor = _GetPixelColor(cap, RsizeBox / 5 + field_x * RsizeBox, RsizeBox * 5 + field_y * RsizeBox);
        //SetCursorPos(RsizeBox / 2 + R_x * RsizeBox, RsizeBox * 2 / 3 + R_y * RsizeBox);
        //printf("$fx %d $fy %d\n", RsizeBox / 2 + field_x * RsizeBox, RsizeBox / 4 + field_y * RsizeBox);
        //if (base != CLR_INVALID) {
        //    result.r = GetRValue(base);
        //    result.g = GetGValue(base);
        //    result.b = GetBValue(base);
        //    printf("color: (%d, %d, %d)\n", result.r, result.g, result.b);
        //}
        //else {
        //    printf("ERRORCOLOR\n");
        //}
    }
    return result;
}

//
COLORREF Reader::_GetPixelColor(const ScreenCapture& cap, int px, int py) {
    if (!cap.valid) {
        //printf("1111\n");
        return CLR_INVALID;
        
    }
    if (px < 0 || py < 0 || px >= cap.w || py >= cap.h) {
        //printf("px %d py %d\n", px, py);
        //printf("2222\n"); 
        return CLR_INVALID;
    
    }

    int i = (py * cap.w + px) * 4;
    BYTE b = cap.pixels[i + 0];
    BYTE g = cap.pixels[i + 1];
    BYTE r = cap.pixels[i + 2];
    return RGB(r, g, b);
}
//
void Reader::_ReleaseCapture(ScreenCapture& cap) {
    if (cap.hdcMem) {
        if (cap.oldBitmap) SelectObject(cap.hdcMem, cap.oldBitmap);
        if (cap.hbm)       DeleteObject(cap.hbm);
        DeleteDC(cap.hdcMem);
        cap.hdcMem = nullptr;
        cap.hbm = nullptr;
        cap.oldBitmap = nullptr;
    }
    if (cap.hdcScreen) {
        ReleaseDC(NULL, cap.hdcScreen);
        cap.hdcScreen = nullptr;
    }
    cap.pixels.clear();
    cap.valid = false;
}

//
//void Reader::get_size(int& h, int& w) {
//    // область захвата — от (_x, _y) до правого нижнего угла экрана
//    printf("Rh %d Rw %d RsH %d RSW %d Rsize %d _x %d _y %d\n", Rh, Rw, RscreenH, RscreenW, RsizeBox, R_x, R_y);
//    ScreenCapture cap = _CaptureScreen(R_x, R_y, RscreenW - R_x, RscreenH - R_y);
//    int field_x = 0, field_y = 0;
//
//    while (1) {
//        RGB color = getColors(cap, field_x, 0);
//        //printf("field_x: %d, color: (%d, %d, %d)\n", field_x, color.r, color.g, color.b);
//        if (color == colors[1]) {
//            field_x++;
//        }
//        else {
//            break;
//        }
//    }
//    //printf("\n");
//    while (1) {
//        RGB color = getColors(cap, 0, field_y);
//        //printf("field_y: %d, color: (%d, %d, %d)\n", field_x, color.r, color.g, color.b);
//        if (color == colors[1]) {
//            field_y++;
//        }
//        else {
//            break;
//        }
//    }
//    _ReleaseCapture(cap);
//
//    printf("X %d Y %d\n", field_x, field_y);
//
//    h = field_y;
//    w = field_x;
//
//    Rh = field_y;
//    Rw = field_x;
//    matrix = new int* [Rh];
//    for (int i = 0; i < Rh; ++i) {
//        matrix[i] = new int[Rw]();
//    }
//    //printf("Rh %d Rw %d RsH %d RSW %d Rsize %d _x %d _y %d\n", Rh, Rw, RscreenH, RscreenW, RsizeBox, R_x, R_y);
//}
//void Reader::get_size2(int& h, int& w) {
//    // область захвата — от (_x, _y) до правого нижнего угла экрана
//    //printf("Rh %d Rw %d RsH %d RSW %d Rsize %d _x %d _y %d\n", Rh, Rw, RscreenH, RscreenW, RsizeBox, R_x, R_y);
//    ScreenCapture cap = _CaptureScreen(R_x, R_y, RscreenW - R_x, RscreenH - R_y);
//    int field_x = 0, field_y = 0;
//
//    while (1) {
//        RGB color = getColors2(cap, field_x, 0);
//        //printf("field_x: %d, color: (%d, %d, %d)\n", field_x, color.r, color.g, color.b);
//        if (color == colors[1]) {
//            field_x++;
//        }
//        else {
//            break;
//        }
//    }
//    //printf("\n");
//    while (1) {
//        RGB color = getColors2(cap, 0, field_y);
//        //printf("field_y: %d, color: (%d, %d, %d)\n", field_x, color.r, color.g, color.b);
//        if (color == colors[1]) {
//            field_y++;
//        }
//        else {
//            break;
//        }
//    }
//    _ReleaseCapture(cap);
//    field_x--;
//    field_y--;
//    printf("X %d Y %d\n", field_x, field_y);
//    if (field_x > 0 && field_y > 0) {
//        h = field_y;
//        w = field_x;
//
//        Rh = field_y;
//        Rw = field_x;
//        matrix = new int* [Rh];
//        for (int i = 0; i < Rh; ++i) {
//            matrix[i] = new int[Rw]();
//        }
//    }
//    else {
//        printf("gtybv\n");
//    }
//    
//    //printf("Rh %d Rw %d RsH %d RSW %d Rsize %d _x %d _y %d\n", Rh, Rw, RscreenH, RscreenW, RsizeBox, R_x, R_y);
//}
//
int** Reader::readScreen() {
    printf("read screen\n");
    //printf("Rh %d Rw %d RsH %d RSW %d Rsize %d _x %d _y %d\n", Rh, Rw, RscreenH, RscreenW, RsizeBox, R_x, R_y);
    ScreenCapture cap = _CaptureScreen(R_x, R_y, Rw * RsizeBox, Rh * RsizeBox);
    
    for (int o = 0; o < Rh; o++) {
        for (int p = 0; p < Rw; p++) {
            //printf("it %d %d\n", o, p);
            RGB color = getColors(cap, o, p);

            for (int t = 0; t < colors.size(); t++) {
                if (color == colors[t]) {
                    //printf("%d ", t - 2);
                    matrix[p][o] = t - 2;
                    break;
                }
            }
            //printf("\n");
        }
    }
    _ReleaseCapture(cap);
    //return nullptr;
    return matrix;
}
int** Reader::readScreen2() {
    printf("read screen\n");
    //printf("Rh %d Rw %d RsH %d RSW %d Rsize %d _x %d _y %d\n", Rh, Rw, RscreenH, RscreenW, RsizeBox, R_x, R_y);
    ScreenCapture cap = _CaptureScreen(R_x, R_y, Rw * RsizeBox, Rh * RsizeBox);

    for (int o = 0; o < Rh; o++) {
        for (int p = 0; p < Rw; p++) {
            //printf("it %d %d\n", o, p);
            RGB color = getColors2(cap, p, o);

            for (int t = 0; t < colors.size(); t++) {
                if (color == colors[t]) {
                    //printf("%d %d %d\n", color.r, color.g, color.b);
                    matrix[o][p] = t - 2;
                    break;
                }
            }
            //printf("\n");
        }
    }
    //printf("++++++++++++++++++++++++++++++\n");
    //printf("mat\n");
    //for (int i = 0; i < Rh; ++i) {
    //    for (int j = 0; j < Rw; ++j) {
    //        printf("%d\t", matrix[i][j]);
    //    }
    //    printf("\n");
    //}
    _ReleaseCapture(cap);
    return matrix;
}
void Reader::free() {
    for (int i = 0; i < Rh; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;

    matrix = nullptr;
    Rh = Rw = 0;
}

int Reader::fundXLH(const ScreenCapture& cap) {
    int _x = 0;
    int status = 0;
    RGB target = { 82, 82, 82 };

    while (1) {
        RGB result = { 0, 0, 0 };
        COLORREF p = _GetPixelColor(cap, _x, RscreenH / 4);
        if (p != CLR_INVALID) {
            result.r = GetRValue(p);
            result.g = GetGValue(p);
            result.b = GetBValue(p);
        }

        if (status == 0 && result == target) {
            status = 1;
        }
        else if ((status == 1 && result != target)) {
            break;
        }
        else if (_x >= RscreenW) {
            _x = -1;
            break;
        }
        else {
            _x++;
        }
    }

    return _x;
}
int Reader::fundYLH(const ScreenCapture& cap, int _x){
    int _y = RscreenH / 4;
    _x++;
    RGB target = { 82, 82, 82 };

    while (1) {
        RGB result = { 0, 0, 0 };
        COLORREF p = _GetPixelColor(cap, _x, _y);
        if (p != CLR_INVALID) {
            result.r = GetRValue(p);
            result.g = GetGValue(p);
            result.b = GetBValue(p);
        }

        if (result == target) {
            _y++;
            break;
        }
        else if (_y < 0 || _y >= RscreenH) {
            _y = -1;
            break;
        }
        else {
            _y--;
            //printf("_y %d\n", _y);
        }
    }

    return _y;
}
void Reader::fundRL(const ScreenCapture& cap, int _xlh, int _ylh, int* _xrl, int* _yrl){
    int status_work = 0;
    int _x = _xlh;
    int _y = _ylh - 1;
    int status = 0;
    RGB target1 = { 82, 82, 82 }; 
    RGB target2 = { 170, 170, 170 };
    while (1) {
        RGB result = { 0, 0, 0 };
        COLORREF p = _GetPixelColor(cap, _x, _y);
        if (p != CLR_INVALID) {
            result.r = GetRValue(p);
            result.g = GetGValue(p);
            result.b = GetBValue(p);
        }

        if (status == 0 && result != target1) {
            status = 1;
            _x--;
        }
        else if (status == 1 && result == target2) {
            _y--;
            break;
        }
        else if(status == 0){
            _x++;
        }
        else if (_x >= RscreenW || _x < 0 || _y >= RscreenH) {
            status_work = 1;
            break;
        }
        else {
            _y++;
        }
    }

    if (status_work == 0) {
        *_xrl = _x;
        *_yrl = _y;
    }
    else {
        *_xrl = -1;
        *_yrl = -1;
    }
}
void Reader::fundRC(const ScreenCapture& cap, int _xlh, int _ylh, int _xrl, int _yrl, int& _r, int& _c) {
    int r = 0, c = 0;
    RGB target1_0 = { 82, 82, 82 };
    RGB target1_1 = { 170, 170, 170 };

    int x1 = _xrl, y1 = _ylh;
    int status1 = 0;

    while (1) {
        RGB result = { 0, 0, 0 };
        COLORREF p = _GetPixelColor(cap, x1, y1);
        if (p != CLR_INVALID) {
            result.r = GetRValue(p);
            result.g = GetGValue(p);
            result.b = GetBValue(p);
        }

        if (result == target1_0) {
            break;
        }
        else if (status1 == 0 && result == target1_1) {
            status1 = 1;
            c++;
        }
        else if (status1 == 1 && result != target1_1) {
            status1 = 0;
        }
        x1--;
    }
    _c = c;
    
    x1 = _xrl, y1 = _ylh;
    status1 = 0;
    while (1) {
        RGB result = { 0, 0, 0 };
        COLORREF p = _GetPixelColor(cap, x1, y1);
        if (p != CLR_INVALID) {
            result.r = GetRValue(p);
            result.g = GetGValue(p);
            result.b = GetBValue(p);
        }

        if (result == target1_1) {//
            break;
        }
        else if (status1 == 0 && result == target1_0) {
            status1 = 1;
            r++;
        }
        else if (status1 == 1 && result != target1_0) {
            status1 = 0;
        }
        y1++;
    }
    _r = r;
}
void Reader::create() {
    matrix = new int* [Rh];
    for (int i = 0; i < Rh; i++) {
        matrix[i] = new int[Rw]();
    }
    printf("!!!!!!h %d w %d", Rh, Rw);
}
int Reader::init(int& sB, int& xX, int& yY, int& rrow, int& ccol) {
    int res = 0;
    ScreenCapture cap = _CaptureScreen(0, 0, RscreenW, RscreenH);
    //printf("?\n");
    int _xLH = fundXLH(cap);
    //printf("??\n");
    int _yLH = fundYLH(cap, _xLH);
    if (_xLH == -1 || _yLH == -1) {
        res = 1;
        printf("ERROR _x _y\n");
    }
    else {
        printf("=== _x %d_y %d\n", _xLH, _yLH);
        xX = _xLH;
        yY = _yLH;
        printf("!!\n");
        int _xRL = 0, _yRL = 0;
        int _row = 0, _col = 0;
        //printf("!\n");
        fundRL(cap, _xLH, _yLH, &_xRL, &_yRL);
        if (_xRL != -1 && _yRL != -1) {
            fundRC(cap, _xLH, _yLH, _xRL, _yRL, _row, _col);
        }
        

        if (_row > 0 && _col > 0) {
            rrow = _row;
            ccol = _col;
            sB = (_xRL - _xLH + 1) / _col;
        }
        else {
            printf("ERROR col row\n");
            res = 1;
        }
    }
    
    
    
    _ReleaseCapture(cap);
    return res;
}
void Reader::set(int& sB, int& xX, int& yY, int& rrow, int& ccol) {
    Rh = rrow;
    Rw = ccol;

    R_x = xX;
    R_y = yY;
    RsizeBox = sB;
}
bool Reader::checkW() {
    bool res = false;
    ScreenCapture cap = _CaptureScreen(0, 0, RscreenW, RscreenH);

    ///
    int X = 1072, Y = 224;
    RGB target = { 240, 240, 240 };
    ///

    RGB color = { 0, 0, 0 };
    
    COLORREF p = _GetPixelColor(cap, X, Y);
    if (p != CLR_INVALID) {
        color.r = GetRValue(p);
        color.g = GetGValue(p);
        color.b = GetBValue(p);
    }
    if (color == target) {
        res = true;
    }

    _ReleaseCapture(cap);
    return res;
}