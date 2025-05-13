
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
| **Visual Studio** | 2019/2022        | Компилятор MSVC / MSVC build |

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

## 🛠️ Сборка c MSVC / Building with MSVC

1. Установите Visual Studio с компонентом "C++ для рабочего стола" / Install Visual Studio with "Desktop C++" workload.
2. Откройте терминал: x64 Native Tools Command Prompt for VS 2022
3. Выполните / Run:
```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

Файл будет создан: / Executable will be created:
```
build\Release\search_engine.exe
```

---

## ✅ Тестирование / Testing

### MinGW
```bash
cmake .. -G "MinGW Makefiles" -DBUILD_TESTS=ON
mingw32-make
ctest
```

### MSVC
```bash
cmake .. -G "Visual Studio 17 2022" -A x64 -DBUILD_TESTS=ON
cmake --build . --config Debug
ctest -C Debug
```

| Режим / Mode       | Команда / Command                     | Назначение / What it does                 |
| ------------------ | ------------------------------------- | ----------------------------------------- |
| **Основной**       | `cmake ..`                            | Сборка основного приложения / Main build |
| **Тестовый**       | `-DBUILD_TESTS=ON`                    | Включает тесты / Enables tests            |
| **Только тесты**   | `-DONLY_TESTS=ON -DBUILD_TESTS=ON`    | Только тесты / Build only tests           |
