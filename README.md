# Отчёт по лабораторной работе №5

## Homework

1. Создайте `CMakeList.txt` для библиотеки *banking*.
2. Создайте модульные тесты на классы `Transaction` и `Account`.
    * Используйте mock-объекты.
    * Покрытие кода должно составлять 100%.
3. Настройте сборочную процедуру на **GitHub Actions**.
4. Настройте Coveralls.io.

### Часть 1

1. Добавляем в репозиторий библиотеку GoogleTest (Gtest)

```sh
$ mkdir third-party
$ git submodule add https://github.com/google/googletest third-party/gtest
$ cd third-party/gtest && git checkout release-1.11.0 && cd ../..
$ git add third-party/gtest
```

2. Создаём минимальный файл `CMakeLists.txt` для сборки библиотеки `banking`

```sh
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(banking)

add_library(Account STATIC Account.cpp Account.h)
add_library(Transaction STATIC Transaction.cpp Transaction.h)
```

3. Добавляем в файл `CMakeLists.txt` модуль для запуска тестов библиотеки `banking`

```sh
if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(third-party/gtest)
  file(GLOB \${PROJECT_NAME}_TEST_SOURCES tests/*.cpp)
  add_executable(check \${\${PROJECT_NAME}_TEST_SOURCES})
  target_link_libraries(check \${PROJECT_NAME} gtest_main)
  add_test(NAME check COMMAND check)
endif()
```

### Часть 2

### Часть 3

### Часть 4
