#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Account.h>
#include <Transaction.h>
#include <iostream>

using ::testing::AtLeast;

// Mock-class for "Account" class 
class MockAccount: public Account
{
    MockAccount(int id, int balance) : Account::Account(id, balance) {}
    MOCK_METHOD(void, Account, (int id, int balance));
    //MOCK_METHOD(void, ~Account, (), (override));
    MOCK_METHOD(int, GetBalance, (), (const, override));
    MOCK_METHOD(void, ChangeBalance, (int diff), (override));
    MOCK_METHOD(void, Lock, (), (override));
    MOCK_METHOD(void, Unlock, (), (override));
    MOCK_METHOD(int, id, (), (const));
};

// Mock-class for "Transaction" class
class MockTransaction: public Transaction
{
    MOCK_METHOD(void, Transaction, ());
    //MOCK_METHOD(void, ~Transaction, (), (override));
    MOCK_METHOD(bool, Make, (Account& from, Account& to, int sum));
    MOCK_METHOD(int, fee, (), (const));
    MOCK_METHOD(void, set_fee, (int fee));
};

// Testing basic features of the "MockAccount" class
TEST(Account, MockAccountTest)
{
    MockAccount m_acc(69, 0);
    
    EXPECT_CALL(m_acc, GetBalance()).Times(AtLeast(2));
    EXPECT_CALL(m_acc, ChangeBalance(testing::_)).Times(AtLeast(1));
    EXPECT_CALL(m_acc, Lock()).Times(AtLeast(1));
    EXPECT_CALL(m_acc, Unlock()).Times(AtLeast(1));
    EXPECT_CALL(m_acc, id()).Times(AtLeast(1));
   // EXPECT_CALL(m_acc, ~Account()).Times(AtLeast(1));
    
    EXPECT_EQ(m_acc.id(), 69);
    EXPECT_EQ(m_acc.GetBalance(), 0);
    m_acc.ChangeBalance(123);
    EXPECT_EQ(m_acc.GetBalance(), 123);
    m_acc.Lock();
    m_acc.Unlock();
}

// Testing basic features of the "MockTransaction" class
TEST(Transaction, MockTransactionTest)
{
    MockTransaction m_tran();
    MockAccount m_acc1(11, 1000);
    MockAccount m_acc2(22, 0);
    
    EXPECT_CALL(m_tran, set_fee(testing::_)).Times(AtLeast(1));
    EXPECT_CALL(m_tran, fee()).Times(AtLeast(2));
    EXPECT_CALL(m_tran, Make(testing::_, testing::_, testing::_)).Times(AtLeast(1));
    
    EXPECT_CALL(m_acc1, Lock()).Times(AtLeast(1));
    EXPECT_CALL(m_acc2, Lock()).Times(AtLeast(1));
    
    EXPECT_CALL(m_acc1, Unlock()).Times(AtLeast(1));
    EXPECT_CALL(m_acc2, Unlock()).Times(AtLeast(1));
    
    EXPECT_CALL(m_acc1, ChangeBalance(testing::_)).Times(AtLeast(1));
    EXPECT_CALL(m_acc1, GetBalance()).Times(AtLeast(4));
    EXPECT_CALL(m_acc1, id()).Times(AtLeast(3));
    
    EXPECT_CALL(m_acc2, ChangeBalance(testing::_)).Times(AtLeast(1));
    EXPECT_CALL(m_acc2, GetBalance()).Times(AtLeast(3));
    EXPECT_CALL(m_acc2, id()).Times(AtLeast(3));
    
    // Checking of "id" values for both accounts
    EXPECT_EQ(m_acc1.id(), 11);
    EXPECT_EQ(m_acc2.id(), 22);
    
    // Checking account balances
    EXPECT_EQ(m_acc1.GetBalance(), 1000);
    EXPECT_EQ(m_acc2.GetBalance(), 0);
    
    // Checking of "fee" value before and after it changes
    EXPECT_EQ(m_tran.fee(), 1);
    m_tran.set_fee(19);
    EXPECT_EQ(m_tran.fee(), 19);
    
    // Checking for the success of the operation
    EXPECT_EQ(m_tran.Make(m_acc1, m_acc2, 123), true);
    
    // Checking of balances after transaction
    EXPECT_EQ(m_acc1.GetBalance(), 858);
    EXPECT_EQ(m_acc2.GetBalance(), 123);
}

// Testing exceptions for "MockAccount" class 
TEST(Account, ExceptionsTest)
{
    MockAccount m_acc(123, 0);
    
    EXPECT_CALL(m_acc, ChangeBalance(testing::_)).Times(AtLeast(1));
    EXPECT_CALL(m_acc, Lock()).Times(AtLeast(2));
    
    // Attempt to change balance while account not locked
    EXPECT_THROW(m_acc.ChangeBalance(), std::runtime_error);
    
    // Attempt to lock a locked account
    m_acc.Lock();
    EXPECT_THROW(m_acc.Lock(), std::runtime_error);
}

// Testing exceptions for "MockTransaction" class
TEST(Transcation, ExceptionsTest)
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
    EXPECT_EQ(m_tran.Make(m_acc1, m_acc2, 100), false);
    
    // Test "Debit" function exception then sum = 0 (fee >= sum)
    m_tran.set_fee(-3060);
    EXPECT_ANY_THROW(m_tran.Make(m_acc1, m_acc2, 100));
    
    // Test if operation "Make" don't make transaction
    m_tran.set_fee(5);
    
    /* Checking for the failure of the operation */
    EXPECT_EQ(m_tran.Make(m_acc1, m_acc2, 499), false);
    
    /* Checking accounts for no changes */
    EXPECT_EQ(m_acc1.GetBalance(), 500);
    EXPECT_EQ(m_acc2.GetBalance(), 0);
}

