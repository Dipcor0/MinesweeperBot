#include "MSF.h"
using namespace std;
//https://minesweeperonline.com/
//программа для авторешения сапера 
// запускаете программу и переключаете на окно 
// окно 200% и лучше left 
// поле должно полность видно
// s остановка
// r перезагрузка ( при победе чтобы дальше играть иначе забагается на полность открытом поле до конца итераций)
// если 500 итерация прошло и вы дальше хотите играть то просто enter
// 



void setConsole() {
    HWND hwnd;
    wchar_t Title[1024];
    GetConsoleTitle(Title, 1024); // Узнаем имя окна
    hwnd = FindWindow(NULL, Title); // Узнаем hwnd окна
    MoveWindow(hwnd, 1270, 400, 650, 600, TRUE);//xnew,ynew,wnew,hnew -новые положение x,y, ширина и высота
}

int main() {
    //while (1) {
    //    POINT p;
    //    if (GetCursorPos(&p)) {
    //        printf("%d %d\n", p.x, p.y);
    //    }
    //}
    //return 0;
    //SetCursorPos(_x, _y);
    
    setConsole();
    Sleep(3000);
    
    
//SetCursorPos(_x, _y);
    //return 0;

    Field field;
    Reader reader(screenH, screenW);
    Writer writer(screenH, screenW);
    printf("x %d y %d sB %d row %d col %d\n", _y, _x, sizeBox, row, col);
    if (reader.init(sizeBox, _x, _y, row, col) == 1) {
        if (reader.init(sizeBox, _x, _y, row, col) == 1) {
            return 0;
        }
    }
    reader.set(sizeBox, _x, _y, row, col);
    
    writer.set(sizeBox, _x, _y, row, col);
    field.set(row, col);

    reader.create();
    field.create();


    char c = ' ';
    int inter = 500;
    int counter = 0;
    //SetCursorPos(_x + sizeBox/2, _y + sizeBox * 2/ 3);
    while (1) {
        //getchar();
        printf("------\n");
        field.input_matrix(reader.readScreen2());
        
        
        printf("inter %d flag: %d\n", counter, field.check());
        //c = getchar();
        if (reader.checkW()) {
            writer.win();
            //counter = inter;
            c = getchar();
        }

        if(field.check() == 0){
            c = 'r';
            //getchar();
        }
        else if (counter == inter) {
            c = getchar();
            counter = 0;
        }
        else {
            counter++;
        }

        //if (!field.check() || counter == inter) {
        //    c = getchar();
        //    counter = 0;
        //}
        //else {
        //    inter++;
        //}
        
        if (c == 'r') {
            //counter = 0;
            writer.reload();

            field.free();
            reader.free();

            if (reader.init(sizeBox, _x, _y, row, col) == 1) {
                if (reader.init(sizeBox, _x, _y, row, col) == 1) {
                    return 0;
                }
            }
            reader.set(sizeBox, _x, _y, row, col);
            writer.set(sizeBox, _x, _y, row, col);
            field.set(row, col);

            reader.create();
            field.create();
            printf("========\n");
            c = ' ';
        }
        else if (c == 's') {
            break;
        }
        else {
            field.action();
            writer.write_action(field.output_matrix());
            //Sleep(35);
            Sleep(10);
        }
    }
    
    field.free();
    reader.free();
    return 0;
}
