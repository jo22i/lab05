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
    MockAccount m_acc(123, 1000);
    
    EXPECT_THROW(m_acc.ChangeBalance(97521), std::runtime_error);
    m_acc.Lock();
    EXPECT_THROW(m_acc.Lock(), std::runtime_error);
}

TEST(Transaction, MockTranTest)
{
    MockTransaction m_t;
    MockAccount m_acc1(1, 1000);
    MockAccount m_acc2(2, 0);
    
    /*EXPECT_CALL(m_acc1, id()).Times(4);
    EXPECT_CALL(m_acc1, Lock()).Times(1);
    EXPECT_CALL(m_acc1, Unlock()).Times(1);
    EXPECT_CALL(m_acc1, GetBalance())
        .Times(4)
        .WillOnce(testing::Return(1000))
        .WillOnce(testing::Return(1000))
        .WillOnce(testing::Return(470))
        .WillOnce(testing::Return(470));
    EXPECT_CALL(m_acc1, ChangeBalance(-530)).Times(1);
    
    EXPECT_CALL(m_acc2, id()).Times(4);
    EXPECT_CALL(m_acc2, Lock()).Times(1);
    EXPECT_CALL(m_acc2, Unlock()).Times(1);
    EXPECT_CALL(m_acc2, GetBalance())
        .Times(3)
        .WillOnce(testing::Return(0))
        .WillOnce(testing::Return(500))
        .WillOnce(testing::Return(500));
    EXPECT_CALL(m_acc2, ChangeBalance(500)).Times(1);*/
    
    EXPECT_CALL(m_t, fee())
        .Times(2)
        .WillOnce(testing::Return(1))
        .WillOnce(testing::Return(30));
    EXPECT_CALL(m_t, set_fee(30)).Times(1);
    EXPECT_CALL(m_t, Make(testing::_, testing::_, testing::_))
        .Times(1)
        .WillOnce(testing::Return(true));
    
    //Checking ids values
    EXPECT_EQ(m_acc1.id(), 1);
    EXPECT_EQ(m_acc2.id(), 2);
    
    //Checking balances values
    EXPECT_EQ(m_acc1.GetBalance(), 1000);
    EXPECT_EQ(m_acc2.GetBalance(), 0);
    
    //Checking and editing fee value
    EXPECT_EQ(m_t.fee(), 1);
    m_t.set_fee(30);
    EXPECT_EQ(m_t.fee(), 30);
    
    EXPECT_TRUE(m_t.Make(m_acc1, m_acc2, 500));
    
    EXPECT_EQ(m_acc1.GetBalance(), 470);
    EXPECT_EQ(m_acc2.GetBalance(), 500);
}
