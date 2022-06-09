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
    EXPECT_NO_TROW(acc.Lock());
    acc.ChangeBalance(9999);
    EXPECT_NO_THROW(acc.Unlock());
    EXPECT_EQ(acc.GetBalance(), 9999);
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
    
    EXPECT_THROW(T.Make(acc1, acc2, 500), std::logic_error);
    EXPECT_THROW(T.Make(acc1, acc2, -1), std::invalid_argument);
    EXPECT_THROW(T.Make(acc1, acc2, 99), std::logic_error);
    
    T.set_fee(51);
    EXPECT_FALSE(T.Make(acc1, acc2, 100));
    
    EXPECT_FALSE(T.Make(acc1, acc2, 950));
}
