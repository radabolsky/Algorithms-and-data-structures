//
// Created by bobyl on 22.03.2022.
//

#ifndef LAB03_HASHTABLE_H
#define LAB03_HASHTABLE_H

#include <iterator>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

struct Node { // Элемент ХТ
    int key; // вес
    Node *down; // ссылка вниз
    Node() : down(nullptr) {} // конструктор по умолчанию
    Node(int k, Node *d = nullptr) : key(k), down(d) {} // конструктор
    ~Node() { delete down; } // Деструктор узла

};


// ИТЕРАТОР ЧТЕНИЯ


struct myiter : public iterator<
        forward_iterator_tag,
        int
        > {

    Node **bct; //массив сегментов
    size_t pos; //позиция в массиве
    Node *Ptr; // Указатель на данные


    myiter(Node *p = nullptr) : bct(nullptr), pos(0), Ptr(p) {} // конструктор по умолчанию

    bool operator == (const myiter & Other) const {
        return Ptr == Other.Ptr; // сравнение итераторов
    }

    bool operator != (const myiter & Other) const {
        return Ptr != Other.Ptr;
    }

    myiter operator++(); //объявление инкремента
    myiter operator++(int) {
        myiter temp(*this); // сохраняем значение
        ++(*this); // Увеличиваем значение
        return temp;
    }

    pointer operator -> () {
        return & Ptr->key; // возвращаем указатель на значение
    }

    reference operator * () {
        return Ptr->key; // возвращаем значение по ссылке
    }

};

// Итератор вставки (универсальный)
template <typename Container, typename Iter = myiter>
class outiter:
public iterator<output_iterator_tag, typename Container::value_type> {
protected:
    Container& container; // контейнер для вставки элементов
    Iter iter; // Текущее значение итератора
public:
    outiter(Container& c, Iter it) : container(c), iter(it) {} // Конструктор
    const outiter<Container>&
        operator = (const typename Container::value_type& value) {
        iter = container.insert(iter, value).first;
        return *this;
    }
    const outiter<Container>& // Присваивание копии - фиктивное
        operator = (const outiter<Container>&) { return *this; }

    outiter<Container>& operator* () {return *this;} // Разыменование - пустая операция
    outiter<Container>& operator++ () {return *this;} // Инкремент - пустая операция
    outiter<Container>& operator++ (int) {return *this;} // Инкремент - пустая операция
};

template <typename Container, typename Iter>
//Функция для создания итератора вставки
inline outiter<Container, Iter> inserter(Container& c, Iter it) {
    return outiter<Container, Iter>(c,it);
}



class HT { //контейнер - хеш-таблица
    static size_t tags; // Счетчик тегов
    char tag; //тег таблицы
    Node **bucket; // массив сегментов
    size_t count = 0; // мощность множества
public:
    static const size_t Buckets = 16; // К-во сегментов в демо-варианте
    using key_type = int;
    using value_type = int;
    using key_compare = equal_to<int>;

    void swap(HT& right) {
        std::swap(tag, right.tag);
        std::swap(bucket, right.bucket);
        std::swap(count, right.count);
    }

    int hash(int k) const { // Хэш-функция
        return (k * (Buckets - 1) + 7) % Buckets;
    }
    size_t bucket_count() {return Buckets;}

    pair<myiter, bool> insert(myiter, int); // вставка

    myiter insert(const int &k, myiter where = myiter(nullptr)) { //Обертка для вставки
        return insert(where, k).first;
    }



    void Display ();

    myiter begin() const;
    myiter end() const {
        return myiter(nullptr);
    };




    pair<myiter, bool> erase(int);

    HT(): tag('A' + tags++), bucket(new Node*[Buckets]) {
        for (int i = 0; i < Buckets; ++i) bucket[i] = nullptr;
    }


    int size() { return count; }

    template<typename MyIt> HT(MyIt, MyIt); //Конструктор из последовательности
    ~HT();



    myiter find(int k) const; // Поиск по ключу

    HT(const HT& right) : HT() {
        for (auto x = right.begin(); x != right.end(); ++x) insert(*x);
    }

    HT(HT&& right) : HT() {
        swap(right);
    }

    HT& operator = (const HT& right) {
        HT temp;
        for (auto x = right.begin(); x != right.end(); ++x) insert(*x);
        swap(temp);
        return *this;
    }

    HT& operator = (HT && right) {
        swap(right);
        return *this;
    }

    HT& operator |= (const HT &);

    HT operator | (const HT & right) const {
        HT result(*this); return(result |= right);
    };

    HT& operator &= (const HT &);

    HT operator & (const HT& right) const {
        HT result(*this);
        return (result |= right);
    }

    HT& operator -= (const HT&);

    HT operator - (const HT & right) const {
        HT result(*this);
        return (result -= right);
    }

    HT& operator ^= (const HT&);

    HT operator ^ (const HT & right) const {
        HT result(*this);
        return (result ^= right);
    }


};

myiter HT::begin() const {//Итератор на начало
    myiter p(nullptr);
    p.bct = bucket;
    for (; p.pos < Buckets; ++p.pos) {
        p.Ptr = bucket[p.pos];
        if (p.Ptr) break; //Выход, если сегмент не пуст, результат - его начало
    }
    return p;
}

myiter myiter::operator++() // Инкремент итератора = шаг по ХТ
{
    if(!Ptr) { //Первое обращение?
        return *this; // Не работает без предварительной устанвоки на ХТ
    }
    else { //Текущий уже выдан
        if(Ptr->down) { // Есть следующий - вниз
            Ptr = Ptr->down;
            return (*this);
        }
        while (++pos < HT::Buckets) {//Поиск очередного элемента
            if(Ptr == bct[pos])
                return *this;
        }
        Ptr = nullptr; //Таблица закончилась
        return *this;
    }
}


void HT::Display() {
    Node** P = new Node* [Buckets];
    for (auto t = 0 ; t < Buckets; ++t) P[t] = bucket[t];
    bool prod = true;
    cout << endl;
    while (prod) {
        prod = false;
        for (auto t = 0; t < Buckets; ++t) {
            if(P[t]) {
                cout << setw(4) << P[t]->key; // выводим ключ
                P[t] = P[t]->down; // спускаемся ниже
                prod = true;
            } else {
                cout << "-";
            }
            cout << endl;
        }
    }
}

// Поиск ключа k с выдачей итератора на него или end()
myiter HT::find(int k) const {
    auto t(hash(k));
    Node *p(bucket[t]);
    while (p) {
        if (p->key == k) return myiter(p);
        else p = p->down;
    }
    return end();
}

pair<myiter, bool> HT::insert(myiter, int k) //Вставка нового значения k
{
    auto t(hash(k));
    Node* p(bucket[t]);
    while (p) {
        if (p->key == k) return make_pair(myiter(p), true);
        else p = p->down;
    }
    bucket[t] = new Node(k, bucket[t]);
    ++count;
    return make_pair(myiter(bucket[t]), true);
}

#endif //LAB03_HASHTABLE_H
