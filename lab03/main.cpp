#include <iostream>
#include <vector>
#include "Hashtable.h"

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

void printLine(int len){
    for (int i = 0; i < len; ++i){
        std::cout << "=";
    }
    std::cout<<"\n";
}

int main() {
    int space = 40;
//  (A & B) ^ (C | D) \ E
    while (true) {
        system("cls");
        HT A{random_iterator(50, 10), random_iterator()};
        HT B{random_iterator(50, 10), random_iterator()};
        HT C{random_iterator(50, 10), random_iterator()};
        HT D{random_iterator(50, 10), random_iterator()};
        HT E{random_iterator(50, 10), random_iterator()};
        HT F;
        std::vector<HT> sets = {A, B, C, D, E};
        char set_name = 'A';
        std::cout << "\nInitial values of vectors (set randomly)\n";
        for (auto set: sets) {
            printLine(40);
            std::cout << "Set " << set_name++ << std::endl;
            set.Display();
        }
        printLine(space);
        std::cout << "Let 's calculate the following expression:" << std::endl;
        std::cout << "(A & B) ^ (C | D) \\ E = F" << std::endl;
        std::cout << "Result: Set " << set_name++ << std::endl;
        F = (A & B) ^ (C | D) - E;
        F.Display();


//
        printLine(space);
        std::cout << "A\n";
        A.Display();

        printLine(space);
        std::cout << "B\n";
        B.Display();

        printLine(space);
        std::cout << "A merge B\n";
        A.Merge(B);
        A.Display();

        printLine(space);
        std::cout << "C\n";
        C.Display();

        printLine(space);
        std::cout << "D\n";
        D.Display();

//
        printLine(space);
        std::cout << "C concat D\n";
        C.Concat(D);
        C.Display();
//
        printLine(space);
        printLine(space);
        std::cout << "E\n";
        E.Display();
        int from, to;
        std::cout << "\nSelect the interval to remove from the sequence\n";
        std::cin >> from >> to;
        std::cout << "E erase(" << from << ", " << to << ")\n";
        E.Erase(from, to);
        E.Display();
        printLine(space);
        int menu;
        std::cout<<"enter any number other than 0 for the next attempt\n"
                   "0 - termination of the program\n";
        std::cin>>menu;
        if (menu == 0) break;
    }

    getchar();
    return 0;
}
