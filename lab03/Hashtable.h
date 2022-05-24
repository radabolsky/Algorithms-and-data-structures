//
// Created by bobyl on 22.03.2022.
//

#ifndef LAB03_HASHTABLE_H
#define LAB03_HASHTABLE_H

#include <iterator>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <valarray>




struct MyNode { // Элемент ХТ
    int key; // вес
    MyNode* down; // ссылка вниз
    MyNode() : down(nullptr), key() {} // конструктор по умолчанию
    explicit MyNode(int k, MyNode *d = nullptr) : key(k), down(d) {} // конструктор
    ~MyNode() { delete down; } // Деструктор узла

};


// ИТЕРАТОР ЧТЕНИЯ
typedef MyNode* HTColumn;

struct readIter: public std::iterator<
        std::forward_iterator_tag,
        int
        > {

    HTColumn *columns; //массив сегментов
    size_t columnIndex; //позиция в массиве
    HTColumn column; // Указатель на данные


    explicit readIter(MyNode* p = nullptr) : columns(nullptr), columnIndex(0), column(p) {} // конструктор по умолчанию

    bool operator == (const readIter & Other) const {
        return column == Other.column; // сравнение итераторов
    }

    bool operator != (const readIter & Other) const {
        return column != Other.column;
    }

    readIter operator++(); //объявление инкремента
    readIter operator++(int) {
        readIter temp(*this); // сохраняем значение
        ++(*this); // Увеличиваем значение
        return temp;
    }

    pointer operator -> () { // указатель на значение
        return & (column->key); // возвращаем указатель на значение
    }

    reference operator * () {
        return column->key; // возвращаем значение по ссылке
    }

};


// Итератор вставки (универсальный)
template <typename Container, typename ReadIter = readIter>
class insertIter:
public std::iterator<std::output_iterator_tag, typename Container::value_type> {
protected:
    Container& container; // контейнер для вставки элементов
    ReadIter readiter; // Текущее значение итератора
public:
    insertIter(Container& container, ReadIter riter) : container(container), readiter(riter) {} // Конструктор итератора вставки

    const insertIter<Container>& // Равенство возвращает адрес итератора вставки контейнера, а получает value_type
        operator = (const typename Container::value_type& value) {
            readiter = container.insert(readiter, value).first;
            return *this;
    }
    const insertIter<Container>& // Присваивание копии - фиктивное
        operator = (const insertIter<Container>&) { return *this; }

    insertIter<Container>& operator* () {return *this;} // Разыменование - пустая операция
    insertIter<Container>& operator++ () {return *this;} // Инкремент - пустая операция
    insertIter<Container>& operator++ (int) {return *this;} // Инкремент - пустая операция
};


template <typename Container, typename Iter>
//Функция для создания итератора вставки
insertIter<Container, Iter> myInserter(Container& c, Iter it) {
    return insertIter<Container, Iter>(c, it);
}



class HT { //контейнер - хеш-таблица
    static size_t tags; // Счетчик тегов
    char tag; //тег таблицы
    HTColumn *buckets; // массив сегментов
    size_t count = 0; // мощность множества
public:
    static const size_t buckets_counter = 16; // К-во сегментов в демо-варианте
    using key_type = int;
    using value_type = int;
    using key_compare = std::equal_to<int>;

    void swap(HT& right) {
        std::swap(tag, right.tag);
        std::swap(buckets, right.buckets);
        std::swap(count, right.count);
    }

    int hash(int k) const { // Хэш-функция
        return (k * (buckets_counter - 1) + 7) % buckets_counter;
    }
    size_t bucket_count() {return buckets_counter;}

    std::pair<readIter, bool> insert(readIter, int); // вставка

    readIter insert(const int &k, readIter where = readIter(nullptr)) { //Обертка для вставки
        return insert(where, k).first;
    }

    void Display(); // Вывод ХТ

    readIter begin() const;
    readIter end() const {
        return readIter(nullptr); // итератор в никуда
    };




    std::pair<readIter, bool> erase(int);

    HT(): tag('A' + tags++), buckets(new HTColumn[buckets_counter]) {
        for (int i = 0; i < buckets_counter; ++i) buckets[i] = nullptr;
    }


    int size() { return count; }

    template<typename MyIt> HT(MyIt, MyIt); //Конструктор из последовательности
    ~HT() {
        --tags;
        for (int bucket = 0; bucket < buckets_counter; ++bucket) delete buckets[bucket];
        delete[] buckets;
    }


    readIter find(int toFind) const; // Поиск по ключу

    HT(const HT& right) : HT() {
        for (readIter i = right.begin(); i != right.end(); ++i) this->insert(*i);
    }

    HT(HT&& right) : HT() { //Конструктор копирования
        swap(right);
    }

    HT& operator = (const HT& right) {
        HT temp;
        for (int & x : right) this->insert(x);
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
        return (result &= right);
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


    void Merge(const HT& right);
};

readIter HT::begin() const {//Итератор на начало
    readIter begin(nullptr); // Итератор чтения
    begin.columns = this->buckets;
    for (; begin.columnIndex < this->buckets_counter; ++begin.columnIndex) { //проходимся по всем колонкам таблицы
        begin.column = buckets[begin.columnIndex];
        if (begin.column) break; //Выход, если сегмент не пуст, результат - его начало
    }
    return begin;
}

readIter readIter::operator++() // Инкремент итератора = шаг по ХТ
{
    if(!column) { //Первое обращение?
        return *this; // Текущая колонка еще не выставлена
    }
    else { //Текущий итератор указывает на элемент из колонки
        if(column->down) { // Есть следующий в колонке - вниз
            column = column->down;
            return (*this);
        }
        while (++columnIndex < HT::buckets_counter) {//Поиск очередной не пустой колонки с элементом
            if (columns[columnIndex]) { //Найден непустая колонка
                column = columns[columnIndex]; // Устанавливаем итератор на голову колонки
                return *this;
            }

        }
        column = nullptr; //Таблица закончилась
        return *this;
    }
}


void HT::Display() {
    HTColumn* toPrint = new HTColumn[buckets_counter]; // Массив колонок
    for (auto i = 0 ; i < buckets_counter; ++i) toPrint[i] = buckets[i];
    bool notAllPrinted = true;
    std::cout << tag << ':' << std::endl;
    while (notAllPrinted) {
        notAllPrinted = false;

        for (auto t = 0; t < buckets_counter; ++t) {

            if(toPrint[t]) {
                std::cout << std::setw(4) << toPrint[t]->key; // выводим ключ
                toPrint[t] = toPrint[t]->down; // спускаемся ниже
                notAllPrinted = true;
            } else {
                 std::cout << std::setw(4) << "-";
            }
        }
        std::cout << std::endl;
    }
}

// Поиск ключа toFind с выдачей итератора на него или end()
readIter HT::find(int toFind) const {
    auto colIndex = hash(toFind);
    HTColumn col = buckets[colIndex];
    while (col) {
        if (col->key == toFind) return readIter(col);
        else col = col->down;
    }
    return end();
}

std::pair<readIter, bool> HT::insert(readIter, int k) //Вставка нового значения k
{
    auto colIndex(hash(k));
    HTColumn elem = buckets[colIndex];
    while (elem) {
        if (elem->key == k) return make_pair(readIter(elem), true);
        else elem = elem->down;
    }
    buckets[colIndex] = new MyNode(k, buckets[colIndex]);
    ++count;
    return make_pair(readIter(buckets[colIndex]), true);
}

template<typename MyIt>
HT::HT(MyIt begin, MyIt end) : HT() {
    for (MyIt iter = begin; iter != end; ++iter) insert(*iter);
}

HT &HT::operator|=(const HT &right) {
    //Copy из библиотеки std копирует в таблицу все значения и игнорирует дубликаты (из-за реализации Колинько)
    // Если скопировать что-то в пустую таблицу, получим просто копирование
    // Но можно и в не пустую, тогда все повторяющиеся элементы в резульатате не повторятся
    // Ее предлагается использовать для реализации объединения множеств
    std::copy(right.begin(), right.end(), // source - Откуда
              myInserter(*this, readIter(nullptr))); // destination - куда

    return *this;
}

HT &HT::operator^=(const HT & right) { // xor = (left + right) - (left * right)
    HT leftTemp(*this);
    *this |= right;
    *this -= (leftTemp & right);
    return *this;
}

HT &HT::operator&=(const HT & right) {
    HT leftTemp;
    this->swap(leftTemp);
    for (auto x : right) {
        if (leftTemp.find(x) != end()) { // Есть пересечение - добавить
            this->insert(x);
        }
    }
    return *this;
}

HT &HT::operator-=(const HT & right) {
    // Берем те элементы, которых нет в right
    HT leftTemp;
    this->swap(leftTemp);
    for (auto x : leftTemp) {
        if (right.find(x) == right.end()) { // в right не нашелся x
            this->insert(x);
        }
    }
    return *this;
}


#endif //LAB03_HASHTABLE_H
