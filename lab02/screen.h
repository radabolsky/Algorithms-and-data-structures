#include <exception>
//
// Created by bobyl on 08.02.2022.
//

#ifndef INC_1_SCREEN_H
#define INC_1_SCREEN_H

const int XMAX = 120; // ширина
const int YMAX = 40; // высота

class point {
public:
    int x, y;

    explicit point(int a = 0, int b = 0) : x(a), y(b) {}; //конструктор точки

    void put_point(int a, int b); // вывод точки (2 варианта)
    void put_point(point p) {
        put_point(p.x, p.y);
    }
    void put_line(int, int, int, int); // вывод линии (2 варианта)
    void put_line(point a, point b) {
        put_line(a.x, a.y, b.x, b.y);
    };

};
void screen_init(); // создание экрана
void screen_destroy(); // удаление экрана
void screen_refresh(); // обновление
void screen_clear(); // очистка

#endif //INC_1_SCREEN_H
