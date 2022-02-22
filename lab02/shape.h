
#include <list>
#include <iostream>
#include "screen.h"
//
// Created by bobyl on 08.02.2022.
//

#ifndef INC_1_SHAPE_H

char screen[YMAX][XMAX];
enum color {black = '*', white = '.'};
// === Поддержка экрана в форме матрицы символов ===
void screen_init() { // все в белый
    for (auto & y : screen)
        for (auto & x: y) x = white;
}

void screen_destroy() {
    for (auto & y: screen)
        for (auto & x : y) x = black;
}

bool on_screen(int a, int b) {
    bool in_x = (0 <= a && a < XMAX);
    bool in_y = (0 <= b && b < YMAX);

    return in_x && in_y;
}

void put_point(int a, int b) {
    if(on_screen(a,b)) {
        screen[b][a] = black;
    }
}

void put_point(point p) {put_point(p.x, p.y);}

void put_line(int x0, int y0, int x1, int y1) {
    int dx = 1; // шаг
    int a = x1 - x0; // ширина
    if (a < 0) {
        dx = -1, a = -a; //Отрицательный шаг по горизонтали
    }

    int dy = 1; // шаг
    int b = y1 - y0; // высота
    if (b < 0) {
        dy = -1, b = -b; //Отрицательный шаг по вертикали
    }

    int double_a = 2*a;
    int double_b = 2*b;

    int x_crit = -b + double_a;
    int eps = 0;

    for (;;){ // Рисование линии
        put_point(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        if (eps <= x_crit) x0 += dx, eps += double_b;
        if (eps >= a || a < b) y0 += dy, eps -= double_a;
    }
}

void put_line(point a, point b) {
    put_line(a.x, a.y, b.x, b.y);
}

void screen_clear() {
    screen_init();
}

void screen_refresh() {
    for (int y = YMAX - 1; 0 <= y ; --y) { // с верхней строки до нижней
        for (auto x: screen[y]) // От левого столбца до правого
            std::cout << x;
        //std::cout << '\n';
    }
}

// === Библиотека фигур ===

struct shape { // виртуальный базовый класс "Фигура"
    static std::list<shape*> shapes; // Список всех фигур
    shape() { // конструктор
        shapes.push_back(this); // Фигура присоединяется к списку
    }
    virtual point north() const = 0; //точки для привязки
    virtual point south() const = 0;
    virtual point west() const = 0;
    virtual point east() const = 0;

    virtual point neast() const = 0;
    virtual point seast() const = 0;
    virtual point nwest() const = 0;
    virtual point swest() const = 0;

    virtual point center() const = 0;

    virtual void draw() = 0; //рисование
    virtual void move(int, int) = 0; // перемещение
    virtual void resize(double) = 0; // Изменение размера
    virtual ~shape() {
        shapes.remove(this);
    };


};

std::list<shape*> shape::shapes; //размещение списка фигур в памяти

void shape_refresh() { // перерисовка всех фигур на экране
    screen_clear();
    for (auto p_sh: shape::shapes) p_sh->draw(); // Динамическое связывание
    screen_refresh();
}

class rotatable : virtual public shape { // Фигуры, пригодные к повороту
public:
    virtual void rotate_left() = 0; // Поворот влево
    virtual void rotate_right() = 0; // Поворот вправо
};

class line : public shape {
    /* Отрезок прямой ["w", "e"]
     * north() определяет точку "выше центра отрезка и так далеко
     * на север, как самая его северная точка", и т.п. */
protected:
    point w, e;

public:
    line(point a, point b) : w(a), e(b) {}; // Произвольная линия по двум точкам
    line(point a, int L) {
        w = point(a.x + L - 1, a.y);
        e = a;
    }
    point north( ) const { return point((w.x+e.x)/2, e.y<w.y? w.y : e.y); }
    point south( ) const { return point((w.x+e.x)/2, e.y<w.y? e.y : w.y); }
    point east( ) const { return point(e.x<w.x? w.x : e.x, (w.y+e.y)/2); }
    point west( ) const { return point(e.x<w.x? e.x : w.x, (w.y+e.y)/2); }
    point neast( ) const { return point(w.x<e.x? e.x : w.x, e.y<w.y? w.y : e.y); }
    point seast( ) const { return point(w.x<e.x? e.x : w.x, e.y<w.y? e.y : w.y); }
    point nwest( ) const { return point(w.x<e.x? w.x : e.x, e.y<w.y? w.y : e.y); }
    point swest( ) const { return point(w.x<e.x? w.x : e.x, e.y<w.y? e.y : w.y); }

    point center ( ) const {
        return point(north().x, east().y);
    }

    void move(int a, int b) { w.x += a; w.y += b; e.x += a; e.y += b; }
    void draw( ) { put_line(w, e); }

    void resize(double d) { // изменение длины линии в d раз
        e.x += (e.x - w.x) * d;
        e.y += (e.y - w.y) * d;
    }

};


// Прямоугольник
class rectangle : public rotatable {
protected:
    point sw, ne;
public:
    rectangle(point a, point b): sw(a), ne(b) {}
    point north( ) const { return point((sw.x + ne.x) / 2, ne.y); }
    point south( ) const { return point((sw.x + ne.x) / 2, sw.y); }
    point east( ) const { return point(ne.x, (sw.y + ne.y) / 2); }
    point west( ) const { return point(sw.x, (sw.y + ne.y) / 2); }
    point neast( ) const { return ne; }
    point seast( ) const { return point(ne.x, sw.y); }
    point nwest( ) const { return point(sw.x, ne.y); }
    point swest( ) const { return sw; }

    point center( ) const {
        return point(north().x, east().y);
    }

    void rotate_right() { //поворот вправо относительно se
        int w = ne.x - sw.x; // учитывается масштаб по осям
        int h = ne.y - sw.y;

        sw.x = ne.x - h*2;
        ne.y = sw.y + w/2;
    }

    void rotate_left() { // Поворот влево относительно sw
      int w = ne.x - sw.x;
      int h = ne.y - sw.y;

      ne.x = sw.x + h * 2;
      ne.y = sw.y + w / 2;
    }
    void move(int a, int b)
    { sw.x += a; sw.y += b; ne.x += a; ne.y += b; }
    void resize(double d)
    { ne.x += (ne.x - sw.x) * d; ne.y += (ne.y - sw.y) * d; }
    void draw( )
    {
        put_line(nwest( ), ne); put_line(ne, seast( ));
        put_line(seast( ), sw); put_line(sw, nwest( ));
    }

};

void up(shape& p, const shape& q) // поместить p над q
{ //Это ОБЫЧНАЯ функция, не член класса! Динамическое связывание!!
    point n = q.north( );
    point s = p.south( );
    p.move(n.x - s.x, n.y - s.y + 1);
}


#define INC_1_SHAPE_H

#endif //INC_1_SHAPE_H
