#include <iostream>
#include <vector>
#include "Hashtable.h"

size_t HT::tags = 0;

int main() {

    std::vector<int> home {1,2,3,3,4,17,18};
    std::vector<int> gome {3, 3, 5,6,17,7};
    HT A {home.begin(), home.end()};
    A.Display();
    A.insert(3);
    A.Display();
    HT B{gome.begin(), gome.end()};
    B.Display();
    HT ht3 = B ^ A;
    ht3.Display();

    HT ht4, ht5, ht6;
    ht4 = A | B;
    ht4.Display();
    ht5 = A & B;
    ht5.Display();
    ht6 = ht4 - ht5;
    ht6.Display();
    return 0;
}
