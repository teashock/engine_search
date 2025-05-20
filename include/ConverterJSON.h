//
// Created by ilyap on 03.05.2025.
//

/**
 * Класс для работы с JSON-файлами
*/

#ifndef CONVERTERJSON_H
#define CONVERTERJSON_H

#include <vector>
#include <iostream>

class ConverterJSON {
public:
    ConverterJSON() = default;

    /**
     * Метод получения содержимого файлов
     * @return возвращает список с содержимым файлов,
     * перечисленных в config.json
     */
    std::vector<std::string> GetTextDocuments();

    /**
     * Метод считывает поле max_responses для определения
     * предельного количества ответов на один запрос
     * @return
     */
    int GetResponsesLimit();

    /**
     * Метод получения запросов из файла requests.json
     * @return возвращает список запросов из файла requests.json
     */
    std::vector<std::string> GetRequests();

    /**
     * Положить в файл answers.json результаты поисковых запросов
     */
    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);
    int GetUpdateTime();

};
#endif