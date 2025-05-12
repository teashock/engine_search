#include "include/ConverterJSON.h"
#include "include/InvertedIndex.h"
#include "include/SearchServer.h"
#include <iostream>


int main() {
    try {
        // 1. Создаём объект ConverterJSON для чтения конфигурации, документов и запросов
        ConverterJSON converter;

        // 2. Получаем документы и запросы
        std::vector<std::string> documents = converter.GetTextDocuments();
        std::vector<std::string> requests = converter.GetRequests();

        // 3. Создаём и заполняем обратный индекс
        InvertedIndex index;
        index.UpdateDocumentBase(documents);

        // 4. Создаём поисковый сервер
        SearchServer search_server(index);

        // 5. Выполняем поиск по запросам
        std::vector<std::vector<RelativeIndex>> search_results = search_server.search(requests);

        // 6. Сохраняем результаты поиска
        std::vector<std::vector<std::pair<int, float>>> answers;
        for (const auto& result : search_results) {
            std::vector<std::pair<int, float>> answer_line;
            for (const auto& entry : result) {
                answer_line.emplace_back(entry.doc_id, entry.rank);
            }
            answers.push_back(answer_line);
        }

        converter.putAnswers(answers);

        std::cout << "The search is completed. The results are saved in \"answers.json\".\n";
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
