
#include <time.h>
#include "Hashtable.h"
#include <chrono>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
using namespace std;

size_t HT::tags = 0;
class random_iterator
{
public:
    typedef std::input_iterator_tag iterator_category;
    typedef double value_type;
    typedef int difference_type;
    typedef double* pointer;
    typedef double& reference;

    random_iterator() : _range(1.0), _count(0) {}
    random_iterator(double range, int count) :
            _range(range), _count(count) {}
    random_iterator(const random_iterator& o) :
            _range(o._range), _count(o._count) {}
    ~random_iterator(){}

    double operator*()const{ return ((rand()/(double)RAND_MAX) * _range); }
    int operator-(const random_iterator& o)const{ return o._count-_count; }
    random_iterator& operator++(){ _count--; return *this; }
    random_iterator operator++(int){ random_iterator cpy(*this); _count--; return cpy; }
    bool operator==(const random_iterator& o)const{ return _count==o._count; }
    bool operator!=(const random_iterator& o)const{ return _count!=o._count; }

private:
    double _range;
    int _count;
};

int main( )
{
    using namespace std::chrono;
    setlocale(LC_ALL, "Russian");
//    srand((unsigned int)7); //Пока здесь константа, данные повторяются
//    srand((unsigned int)time(nullptr)); //Разблокировать для случайных данных
    auto rand = [ ] (int d) { return std::rand( )%d; }; //Лямбда-функция!
    ofstream fout;
    fout.open("in.txt");
    cout<<"start\n";

    for(int p = 10; p< 2500; p += 10)
    {
        try {
            int middle_power = 0, set_count = 0;
            auto Used = [&](HT &t) {
                middle_power += t.size();
                ++set_count;
            };
            cout << endl << p << endl;
//        int middle_power = 0, set_count = 0;
//        auto Used = [&] (MyCont & t){ middle_power += t.Power( ); ++set_count; };
            HT A{random_iterator(p * 2, p), random_iterator()};
            HT B{random_iterator(p * 2, p), random_iterator()};
            HT C{random_iterator(p * 2, p), random_iterator()};
            HT D{random_iterator(p * 2, p), random_iterator()};
            HT E{random_iterator(p * 2, p), random_iterator()};
            HT F{random_iterator(p * 2, p), random_iterator()};
//        A.Display();
            auto t1 = std::chrono::high_resolution_clock::now();
            A & B;
            Used(A);
            Used(B);
            C | D;
            Used(C);
            Used(D);
            (A & B) ^ (C | D);
            Used(A);
            Used(B);
            Used(C);
            Used(D);
            (A & B) ^ (C | D) - E;
            Used(A);
            Used(B);
            Used(C);
            Used(D);
            Used(E);

            F.Merge(E);
            Used(F);
            Used(E);
            A.Concat(B);
            Used(A);
            Used(B);
            auto t2 = std::chrono::high_resolution_clock::now();
            auto dt = duration_cast<duration<double>>(t2 - t1);
            middle_power /= set_count;
            fout << middle_power << ' ' << dt.count() << endl;
        }
        catch (std::exception ex){
            ex.what();
        }
    }
    cout << "The end\n";
    fout.close();
    std::cin.get( );
    return 0;
}