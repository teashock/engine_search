//
// Created by ilyap on 06.05.2025.
//

#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#include <iostream>
#include <vector>
#include <map>
#include <mutex>

struct Entry {
    size_t doc_ID, count;
    //For tests
    bool operator == (const Entry& other) const {
        return (doc_ID == other.doc_ID && count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;

    /**
    * Обновить или заполнить базу документов, по которой будем совершать поиск
    * @param texts_input содержимое документов
    */
    void UpdateDocumentBase(std::vector<std::string> input_docs);

    /**
     * Метод определяет количество вхождения словаря word в загруженной базе документов
     * @param word слово, частоту вхождений которого необходимо определить
     * @return возвращает подготовленный список с частотой слов
     */
    std::vector<Entry> GetWordCount(const std::string& word);

private:
    std::vector<std::string> docs; // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
    std::mutex freq_mutex;
};

#endif //INVERTEDINDEX_H
