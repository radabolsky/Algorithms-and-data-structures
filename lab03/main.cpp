#include <iostream>
#include <vector>
#include "Hashtable.h"

size_t HT::tags = 0;

int main() {

    std::vector<int> home {1,2,3,3,4,17,18};
    std::vector<int> gome {3, 3, 5,6,17,7};
    HT A {home.begin(), home.end()};
    A.Display();
    A.eraseElem(3);
    A.Display();
    A.eraseElem(4);
    A.Display();
    HT B{gome.begin(), gome.end()};
    B.Display();
    HT C = A ^ B;
    C.Display();

    A.Display();
    A.Erase(1, 3);
    A.Display();

    //TODO implement Erase, Concat, Merge

    return 0;
}
