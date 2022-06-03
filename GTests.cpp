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
  
}
