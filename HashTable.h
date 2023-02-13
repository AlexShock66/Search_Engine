// owner: Jay Kynerd
// 1.0: HashTable, rule of three
// 2.0: insert, remove, exists
// 3.0: get, at

#ifndef SEARCH_ENGINE_POINTERPALS_HASHTABLE_H
#define SEARCH_ENGINE_POINTERPALS_HASHTABLE_H

#include <list>
#include <cmath>
#include <string>
#include <functional>

using namespace std;

template <typename K, typename V>
class HashTable {
private:
    int size;
    std::list<V> *table;
    int hash(K key) {
        unsigned long index = 0;
        int counter = 0;
        for (const auto& element: key) {
            index += (element * std::pow(counter++, 2));
        }
        index %= size;
        return (int) index;
    }
public:
    HashTable() = default;
    explicit HashTable(int tableSize) {
        table = new std::list<V>[tableSize];
        size = tableSize;
    };
    HashTable(const HashTable &copy) {
        this->size = copy.size;
        this->table = copy.table;
    };
    HashTable& operator=(const HashTable &copy) {
        if (this == &copy) {
            return *this;
        }
        for (int i = 0; i < copy.table->size(); i++) {
            auto temp = copy.table[i];
            for (const auto &copyMe: temp) {
                this->table[i].push_back(copyMe);
            }
        }
        this->size = copy.size;
        return *this;
    };
    ~HashTable() {
        delete [] table;
    };
    void insert(K key, V value) {
        table[hash(key)].push_back(value);
    };
    void remove(K key, V value) {
        table[hash(key)].erase(value);
    };
    bool exists(K key) {
        return !table[hash(key)].empty();
    };
    V& get(K key, int index = 0) {
        if (index > 0) {
            auto it = table[hash(key)].begin();
            it = std::next(it, index);
            return *it;
        }
        return *table[hash(key)].begin();
    };
    V& at(int index) {
        return *table[index].begin();
    };
};

#endif //SEARCH_ENGINE_POINTERPALS_HASHTABLE_H
