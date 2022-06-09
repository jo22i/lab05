[![Coverage Status](https://coveralls.io/repos/github/jo22i/lab05/badge.svg?branch=master)](https://coveralls.io/github/jo22i/lab05?branch=master)

[![GMock_Tests](https://github.com/jo22i/lab05/actions/workflows/action.yml/badge.svg)](https://github.com/jo22i/lab05/actions/workflows/action.yml)

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

```cmake
cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(banking)

add_library(banking STATIC Account.cpp Transaction.cpp)
```

### Часть 2

1. Создаём в главной директории файл *tests.cpp*

```cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Account.h"
#include "Transaction.h"
#include <iostream>

// Mock-class for "Account" class 
struct MockAccount: public Account
{
    MockAccount(int id, int balance) : Account::Account(id, balance) {}
    MOCK_METHOD(int, GetBalance, (), (const, override));
    MOCK_METHOD(void, ChangeBalance, (int diff), (override));
    MOCK_METHOD(void, Lock, (), (override));
    MOCK_METHOD(void, Unlock, (), (override));
    MOCK_METHOD(int, id, ());
};

// Mock-class for "Transaction" class
struct MockTransaction: public Transaction
{
    MOCK_METHOD(bool, Make, (Account & from, Account & to, int sum));
    MOCK_METHOD(int, fee, ());
    MOCK_METHOD(void, set_fee, (int fee));
};

// Testing basic features of the "MockAccount" class
TEST(Account, MockAccTest)
{
    MockAccount m_acc(123, 0);
    
    EXPECT_CALL(m_acc, id())
        .Times(1)
        .WillOnce(testing::Return(123));
    EXPECT_CALL(m_acc, Lock()).Times(1);
    EXPECT_CALL(m_acc, Unlock()).Times(1);
    EXPECT_CALL(m_acc, GetBalance())
        .Times(2)
        .WillOnce(testing::Return(0))
        .WillOnce(testing::Return(9999));
    EXPECT_CALL(m_acc, ChangeBalance(9999)).Times(1);
    
    m_acc.id();
    m_acc.GetBalance();
    m_acc.Lock();
    m_acc.ChangeBalance(9999);
    m_acc.Unlock();
    m_acc.GetBalance();
}

TEST(Account, Exceptions)
{
    Account acc(123, 1000);
    
    EXPECT_THROW(acc.ChangeBalance(97521), std::runtime_error);
    acc.Lock();
    EXPECT_THROW(acc.Lock(), std::runtime_error);
}

TEST(Account, Test)
{
    Account acc(123, 1000);
    
    EXPECT_EQ(acc.id(), 123);
    EXPECT_EQ(acc.GetBalance(), 1000);
    EXPECT_NO_THROW(acc.Lock());
    acc.ChangeBalance(234);
    EXPECT_NO_THROW(acc.Unlock());
    EXPECT_EQ(acc.GetBalance(), 1234);
}

TEST(Transaction, MockTranTest)
{
    MockTransaction m_t;
    MockAccount m_acc1(1, 1000);
    MockAccount m_acc2(2, 0);
    
    EXPECT_CALL(m_t, fee()).Times(2);
    EXPECT_CALL(m_t, set_fee(30)).Times(1);
    EXPECT_CALL(m_t, Make(testing::_, testing::_, testing::_)).Times(1);
    
    m_t.fee();
    m_t.set_fee(30);
    m_t.fee();
    m_t.Make(m_acc1, m_acc2, 500);
}

TEST(Transaction, Test)
{
    Account acc1(1, 1000);
    Account acc2(2, 0);
    Transaction T;
    
    //Checking ids values
    EXPECT_EQ(acc1.id(), 1);
    EXPECT_EQ(acc2.id(), 2);
    
    //Checking balances values
    EXPECT_EQ(acc1.GetBalance(), 1000);
    EXPECT_EQ(acc2.GetBalance(), 0);
    
    //Checking and editing fee value
    EXPECT_EQ(T.fee(), 1);
    T.set_fee(30);
    EXPECT_EQ(T.fee(), 30);
    
    EXPECT_TRUE(T.Make(acc1, acc2, 500));
    
    EXPECT_EQ(acc1.GetBalance(), 470);
    EXPECT_EQ(acc2.GetBalance(), 500);
}

TEST(Transaction, Exceptions)
{
    Account acc1(1, 1000);
    Account acc2(2, 0);
    Transaction T;
    
    EXPECT_THROW(T.Make(acc1, acc1, 500), std::logic_error);
    EXPECT_THROW(T.Make(acc1, acc2, -1), std::invalid_argument);
    EXPECT_THROW(T.Make(acc1, acc2, 99), std::logic_error);
    
    T.set_fee(51);
    EXPECT_EQ(T.fee(), 51);
    
    EXPECT_FALSE(T.Make(acc1, acc2, 100));
    
    EXPECT_FALSE(T.Make(acc1, acc2, 950));
}
```

2. Настраиваем файл *CMakeLists.txt* в главной директории для сборки тестов

```cmake
cmake_minimum_required(VERSION 3.22)

SET(COVERAGE OFF CACHE BOOL "Coverage")
SET(CMAKE_CXX_COMPILER "/usr/bin/g++")

project(GTest_tests)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/third-party/gtest" "gtest")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/banking)

add_executable(RunExec ${CMAKE_CURRENT_SOURCE_DIR}/tests.cpp)

if (COVERAGE)
    target_compile_options(RunExec PRIVATE --coverage)
    target_link_libraries(RunExec PRIVATE --coverage)
endif()

target_include_directories(RunExec PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/banking)

target_link_libraries(RunExec PRIVATE gtest gtest_main gmock_main banking)
```

### Часть 3

Создаём файл *action.yml* для сборки и тестирования программы и добавляем в него необходимые команды для проверки покрытия кода

```yml
name: GMock_Tests
on:
  push:
  
jobs:
  Build_Banking:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: build *banking* lib
        shell: bash
        run: |
          git submodule update --init
          cmake -H. -B_build
          cmake --build _build
        working-directory: banking
          
  Testing_With_GMock:
    runs-on: ubuntu-latest
    needs: Build_Banking
    steps:
      - uses: actions/checkout@v3
      
      - name: update
        run:  |
          git submodule update --init
          sudo apt install lcov
          sudo apt install g++-7
      
      - name: test
        shell: bash
        run: |
          mkdir _build && cd _build
          CXX=/usr/bin/g++-7 cmake -DCOVERAGE=1 ..
          cmake --build .
          ./RunExec
          lcov -t "QuickSort" -o lcov.info -c -d .
          lcov --remove lcov.info '/home/runner/work/lab05/lab05/third-party/gtest/*' -o lcov.info ###
          lcov --remove lcov.info '/usr/include/*' -o lcov.info
          
      - name: CovBeg
        uses: coverallsapp/github-action@master
        with:
          github-token: ${{ secrets.github_token }}
          parallel: true
          path-to-lcov: ./_build/lcov.info
          coveralls-endpoint: https://coveralls.io
          
      - name: CovFin
        uses: coverallsapp/github-action@master
        with:
         github-token: ${{ secrets.github_token }}
         parallel-finished: true
```

### Часть 4

Для насторйки `Coveralls.io` включаем на сайте наш репозитроий, ждём окончания сборки и копируем необходимую команду для отображения покрытия в *README.md*.
