#include <iostream>
#include "screen.h"
#include "shape.h"



class cross : public shape {
protected:
    point c_center;
    int size;
public:
    cross(): shape(), c_center(0), size(0) {};
    cross(point c, int s): c_center(c), size(s) {
        if (on_screen(c)){
            c_center = c;
            size = s;
        }
        else throw CreationError("cross creation error");
    }
    point north() const {return point(c_center.x, c_center.y + size);}; //точки для привязки
    point south() const {return point(c_center.x, c_center.y - size);};
    point west() const {return point(c_center.x - size, c_center.y);};
    point east() const {return point(c_center.x + size, c_center.y);};

    point neast() const {return point(c_center.x + size, c_center.y + size);};
    point seast() const {return point(c_center.x + size, c_center.y - size);};
    point nwest() const {return point(c_center.x - size, c_center.y + size);};
    point swest() const {return point(c_center.x - size, c_center.y - size);};

    point center() const {return c_center;};


    void move(int a, int b); // перемещение
    void resize(double d); // Изменение размера

    void draw(); // переопределим рисование
};

class andrew_cross : public cross {

public:
    andrew_cross() : cross(){}
    andrew_cross(point c, int s): cross(c, s) {};
    void draw();
};
void andrew_cross::draw() {
    put_line(neast(), swest());
    put_line(nwest(), seast());
}



void cross ::draw() {
    put_line(north(), south());
    put_line(east(), west());
}

void cross::resize(double d) {
    size *= d;
}

void cross::move(int a, int b) {
    c_center.x += a;
    c_center.y += b;
    if (!on_screen(*this)) {
        c_center.x -= a;
        c_center.y -= b;
        throw CantMove("After move cross will be out of screen!");
    }
}


// в центр
void into(shape &p, const shape &q) {
    point p_c = p.center();
    point q_c = q.center();

    p.move(q_c.x - p_c.x, q_c.y - p_c.y);
}
// слева
void left(shape &p, const shape &q) {
    point e = q.west();
    point w = p.east();
    p.move(e.x - w.x - 1, e.y - w.y);
}
// справа
void right(shape &p, const shape &q) {
    point w = q.east();
    point e = p.west();
    p.move(w.x - e.x + 1, w.y - e.y);
}

class myshape : public rectangle {
    int w, h;
    line l_eye; // Левый глаз
    line r_eye; // Правый глаз
    line mouth; // рот
public:
    myshape(): rectangle(){}
    myshape(point, point);
    void draw() ;
    void move(int, int);
    void resize(double) {}
};

myshape ::myshape(point a, point b) :
    rectangle(a, b),
    w(neast().x - swest().x + 1),
    h(neast().y - swest().y + 1),
    l_eye(point(swest().x +2, swest().y + h * 3/4), 2),
    r_eye(point(swest().x + w - 4, swest().y + h * 3/4), 2),
    mouth(point(swest().x + 2, swest().y + h/4), w - 4)
{}

void myshape :: draw() {
    rectangle :: draw(); // Контур лица

    point nose = center();

    put_point(nose);
}

void myshape :: move(int a, int b) {
    try {
        rectangle::move(a, b);
        l_eye.move(a, b);
        r_eye.move(a, b);
        mouth.move(a, b);
    } catch (CantMove) { //Если неможем подвинуть прямоугольник - не можем подвинуть и фигуру
        throw CantMove("Cant move rectangle => CantMove myshape!");
    }
}

int main() {

    setlocale(LC_ALL, "Rus");

    screen_init();

    // ==1. Объявление набор фигур ==

//    line brim(point(4,1), 17); // козырек
//    myshape face(point(25,1), point(25+12,1+8));
//    cross left_cross(point(65, 6), 5);
//    cross right_cross(point(107, 6), 5);
//    andrew_cross hat_cross(point(45, 6), 3);

    rectangle hat;
    line brim;
    myshape face;
    cross left_cross;
    cross right_cross;
    andrew_cross hat_cross;

    try{
        rectangle test(point(1000,3), point(5+14,3+5));
        hat = test;
    }
    catch(CreationError &ex){
        cout << "==" << endl << ex.what() << endl;
        cout << "The figure was replaced with a reference one" << endl <<"=="<< endl;
        rectangle n_fig(point(5,3), point(5+14,3+5));
        hat = n_fig;
    }

    try{
        line l_test(point(4000,1), 17);
        brim = l_test;
    }
    catch(CreationError &ex){
        cout << "==" << endl << ex.what() << endl;
        cout << "The figure was replaced with a reference one" << endl <<"=="<< endl;
        line l_fig(point(4,1), 17);
        brim = l_fig;
    }

    try{
        myshape l_test(point(5,1), point(25+12,1+8));
        face = l_test;
    }
    catch(CreationError &ex){
        cout << "==" << endl << ex.what() << endl;
        cout << "The figure was replaced with a reference one" << endl <<"=="<< endl;
        myshape l_test(point(25, 1), point(25 + 12, 1 + 8));
        face = l_test;
    }

    try{
        cross l_test(point(650, 6), 5);
        left_cross = l_test;
    }
    catch(CreationError &ex){
        cout << "==" << endl << ex.what() << endl;
        cout << "The figure was replaced with a reference one" << endl <<"=="<< endl;
        cross l_test(point(65, 6), 5);
        left_cross = l_test;
    }

    try{
        cross l_test(point(1070, 6), 5);
        right_cross = l_test;
    }
    catch(CreationError &ex){
        cout << "==" << endl << ex.what() << endl;
        cout << "The figure was replaced with a reference one" << endl <<"=="<< endl;
        cross l_test(point(107, 6), 5);
        right_cross = l_test;
    }

    try{
        andrew_cross l_test(point(450, 6), 3);
        hat_cross = l_test;
    }
    catch(CreationError &ex){
        cout << "==" << endl << ex.what() << endl;
        cout << "The figure was replaced with a reference one" << endl <<"==" << endl;
        andrew_cross l_test(point(45, 6), 3);
        hat_cross = l_test;
    }


    //== 2.Подготовка к сборке ==
    hat.rotate_right( );

    brim.resize(3.0);
    face.resize(2.0);

//    //== 3.Сборка изображения ==

    face.move(10, 10); // Лицо - в исходное положение
    up(brim, face);
    up(hat, brim);
    into(hat_cross, hat);
    left(left_cross, face);
    right(right_cross, face);
    shape_refresh( );
    std::cout << "=== Ready! ===\n";

// == 4. Проверка исключений ==
//    {
//        std::cout << "OutOfScreen Exception Check:" << std::endl;
//        std::cout << "Make new point in [1000][1000]" << std::endl;
//        put_point(1000, 1000);
//        shape_refresh();
//    }
    std::cin.get();

    std::cout << "CantMove Exception Check: " << std::endl;
    std::cout << "Move face by 100 in x & y axis" << std::endl;

    try {
        face.move(100, 100);
    } catch (CantMove &ex) {
        std::cout << ex.what() << std::endl;
    }
    shape_refresh();
    std::cin.get();

    return 0;
}
