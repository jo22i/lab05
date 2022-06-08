#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Account.h"
#include "Transaction.h"
#include <iostream>

/*using ::testing::Return;
using ::testing::_;
using ::testing::Times;
using ::testing::WillOnce;
using ::testing::WillRepeatedly;*/

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
TEST(AccTest, MockAccountTest)
{
    MockAccount m_acc(69, 0);
    
    EXPECT_CALL(m_acc, GetBalance())
        .Times(2)
        .WillOnce(testing::Return(0))
        .WillOnce(testing::Return(123));
    EXPECT_CALL(m_acc, ChangeBalance(testing::_)).Times(1);
    EXPECT_CALL(m_acc, Lock()).Times(1);
    EXPECT_CALL(m_acc, Unlock()).Times(1);
    EXPECT_CALL(m_acc, id())
        .Times(1)
        .WillOnce(testing::Return(69));
    
    m_acc.id();
    m_acc.GetBalance();
    m_acc.ChangeBalance(123);
    m_acc.GetBalance();
    m_acc.Lock();
    m_acc.Unlock();
}

// Testing basic features of the "MockTransaction" class
TEST(TranscationTest, MockTransactionTest)
{
    MockTransaction m_tran();
    MockAccount m_acc1(11, 1000);
    MockAccount m_acc2(22, 0);
    
    EXPECT_CALL(m_tran, set_fee(testing::_)).Times(1);
    EXPECT_CALL(m_tran, fee())
        .Times(2)
        .WillOnce(Return(1))
        .WillOnce(Return(19));
    EXPECT_CALL(m_tran, Make(testing::_, testing::_, testing::_)).Times(1);
    
    EXPECT_CALL(m_acc1, Lock()).Times(1);
    EXPECT_CALL(m_acc2, Lock()).Times(1);
    
    EXPECT_CALL(m_acc1, Unlock()).Times(1);
    EXPECT_CALL(m_acc2, Unlock()).Times(1);
    
    EXPECT_CALL(m_acc1, ChangeBalance(testing::_)).Times(1);
    EXPECT_CALL(m_acc1, GetBalance()).Times(4);
    EXPECT_CALL(m_acc1, id())
        .Times(3)
        .WillRepeatedly(testing::Return(11));
    
    EXPECT_CALL(m_acc2, ChangeBalance(testing::_)).Times(1);
    EXPECT_CALL(m_acc2, GetBalance()).Times(3);
    EXPECT_CALL(m_acc2, id())
        .Times(3)
        .WillRepeatedly(testing::Return(22));
                  
    // Checking of "id" values for both accounts
    m_acc1.id();
    m_acc2.id();
    
    // Checking account balances
    EXPECT_EQ(m_acc1.GetBalance(), 1000);
    EXPECT_EQ(m_acc2.GetBalance(), 0);
    
    // Checking of "fee" value before and after it changes
    EXPECT_EQ(m_tran.fee(), 1);
    m_tran.set_fee(19);
    EXPECT_EQ(m_tran.fee(), 19);
    
    // Checking for the success of the operation
    EXPECT_TRUE(m_tran.Make(m_acc1, m_acc2, 123));
    
    // Checking of balances after transaction
    EXPECT_EQ(m_acc1.GetBalance(), 858);
    EXPECT_EQ(m_acc2.GetBalance(), 123);
}

// Testing exceptions for "MockAccount" class 
TEST(AccTest, ExceptionsTest)
{
    MockAccount m_acc(123, 0);
    
    // Attempt to change balance while account not locked
    EXPECT_THROW(m_acc.ChangeBalance(), std::runtime_error);
    
    // Attempt to lock a locked account
    EXPECT_NO_THROW(m_acc.Lock());
    EXPECT_THROW(m_acc.Lock(), std::runtime_error);
}

// Testing exceptions for "MockTransaction" class
TEST(TranscationTest, ExceptionsTest)
{
    MockAccount m_acc1(1, 500);
    MockAccount m_acc2(2, 0);
    MockTransaction m_tran;
    
    // Test first "if" exception
    EXPECT_THROW(m_tran.Make(m_acc1, m_acc1, 123), std::logic_error);
    
    // Test second "if" exception
    EXPECT_THROW(m_tran.Make(m_acc1, m_acc2, -5), std::invalid_argument);
    
    // Test third "if" exception
    EXPECT_THROW(m_tran.Make(m_acc1, m_acc2, 99), std::logic_error);
    
    // Test fourth "if" exception
    m_tran.set_fee(51);
    EXPECT_EQ(m_tran.fee(), 51);
    EXPECT_FALSE(m_tran.Make(m_acc1, m_acc2, 100));
    
    // Test "Debit" function exception then sum = 0 (fee >= sum)
    m_tran.set_fee(-3060);
    EXPECT_EQ(m_tran.fee(), -3060);
    EXPECT_ANY_THROW(m_tran.Make(m_acc1, m_acc2, 100));
    
    // Test if operation "Make" don't make transaction
    m_tran.set_fee(5);
    EXPECT_EQ(m_tran.fee(), 5);
    
    /* Checking for the failure of the operation */
    EXPECT_FALSE(m_tran.Make(m_acc1, m_acc2, 499));
    
    /* Checking accounts for no changes */
    EXPECT_EQ(m_acc1.GetBalance(), 500);
    EXPECT_EQ(m_acc2.GetBalance(), 0);
}

