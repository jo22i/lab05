#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "./banking/Account.h"
#include "./banking/Transaction.h"

class MockAccount: public Account
{
    MOCK_METHOD(Account, (int id, int balance));
    MOCK_METHOD(~Account, (), (override));
    MOCK_METHOD(GetBalance, (), (override));
    MOCK_METHOD(ChangeBalance, (int diff), (override));
    MOCK_METHOD(Lock, (), (override));
    MOCK_METHOD(Unlock, (), (override));
    MOCK_METHOD(id, ());
}

class MockTransaction: public Transaction
{
    MOCK_METHOD(Transaction, ());
    MOCK_METHOD(~Transaction, (), (override));
    MOCK_METHOD(Make, (Account& from, Account& to, int sum));
    MOCK_METHOD(fee, ());
    MOCK_METHOD(set_fee, (int fee));
}

TEST(Account, MockTest)
{
    MockAccount m_acc(69, 0);
    EXPECT_CALL(m_acc, GetBalance()).Times(AtLeast(2));
    EXPECT_CALL(m_acc, ChangeBalance()).Times(AtLeast(1));
    EXPECT_CALL(m_acc, Lock()).Times(AtLeast(1));
    EXPECT_CALL(m_acc, Unlock()).Times(AtLeast(1));
    EXPECT_CALL(m_acc, id()).Times(AtLeast(1));
    EXPECT_CALL(m_acc, ~Account()).Times(AtLeast(1));
    
    EXPECT_EQ(m_acc.id(), 69);
    EXPECT_EQ(m_acc.GetBalance(), 0);
    m_acc.ChangeBalance(123);
    EXPECT_EQ(m_acc.GetBalance(), 123);
    m_acc.Lock();
    m_acc.Unlock();
}
