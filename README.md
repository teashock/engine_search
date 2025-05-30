
# 🔍 Search Engine — C++ поисковый движок (консольная версия) / C++ Console Search Engine

## 📄 Описание / Description
C++ реализация простого поискового движка, представляющего собой консольное приложение (исполняемый файл, запускаемый на любом сервере или компьютере), осуществляющее поиск и имеющее возможность настройки через файлы формата JSON. Применённые решения можно впоследствии встроить в поисковую систему, работающую через вэб-интерфейс.

A simple C++ search engine implemented as a console application (executable, runnable on any server or PC), performing search operations and configurable via JSON files. The implemented architecture can later be integrated into a web-based search engine.

---

## ⚙️ Принцип работы / How It Works
1. В `bin/config.json` задаются файлы, по которым будет производиться поиск.
2. Движок индексирует указанные файлы.
3. Пользователь формирует запрос в `bin/requests.json`.
4. Запрос превращается в список слов.
5. Находятся документы, содержащие все слова из запроса.
6. Результаты ранжируются и сортируются. Максимум задаётся в `max_responses`.
7. Ответы сохраняются в `answers.json`.
-
1. Files to be searched are listed in `bin/config.json`.
2. The engine indexes all listed files.
3. Queries are passed via `bin/requests.json`.
4. Each query is tokenized into words.
5. Documents containing all query words are selected.
6. Results are ranked, sorted, and limited by `max_responses`.
7. Output is saved to `answers.json`.

---

## 🧩 Особенности и технологии / Features and Technologies
- Многопоточная индексация / Multithreaded indexing
- Обработка JSON-запросов / JSON query processing
- Ранжирование результатов / Result ranking
- Модульные тесты (GoogleTest) / Unit testing (GoogleTest)
- Использование [nlohmann/json](https://github.com/nlohmann/json)

---

## 🛠️ Инструменты / Tools

| Инструмент / Tool | Версия / Version | Назначение / Purpose |
| ----------------- | ---------------- | --------------------- |
| **CMake**         | ≥ 3.15           | Сборочная система / Build system |
| **Git**           | Любая / Any      | Клонирование / Clone repo |
| **MinGW**         | GCC ≥ 9.0        | Компилятор / Windows compiler |

Проверьте наличие утилит:
Ensure these are available in terminal:
```bash
g++ --version
cmake --version
mingw32-make --version
```

---

## 🛠️ Сборка MinGW / Building with MinGW

```bash
git clone https://github.com/teashock/engine_search.git
cd engine_search

mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
```

По завершении: / After build:
```
build/search_engine.exe
```
Также будут скопированы / Also copied: `config.json`, `requests.json`

---

## 🛠️ Запуск программы/Run

При запуске `search_engine.exe` будет создан файл `answer.json`, содержащий результаты обработки запросов.
Этот файл можно открыть с помощью любого текстового редактора, например Блокнота или VS Code.

When you run `search_engine.exe`, it will create a file named `answer.json` containing the processed query results.
You can open this file with any text editor, such as Notepad or VS Code.

---

## ✅ Тестирование / Testing

```bash
cmake .. -G "MinGW Makefiles" -DBUILD_TESTS=ON
mingw32-make
ctest
```

| Режим / Mode       | Команда / Command                     | Назначение / What it does                 |
| ------------------ | ------------------------------------- | ----------------------------------------- |
| **Основной**       | `cmake ..`                            | Сборка основного приложения / Main build  |
| **Тестовый**       | `-DBUILD_TESTS=ON`                    | Включает тесты / Enables tests            |
| **Только тесты**   | `-DONLY_TESTS=ON -DBUILD_TESTS=ON`    | Только тесты / Build only tests           |
