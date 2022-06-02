#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "./banking/Account.h"
#include "./banking/Transaction.h"

//Добавить модули для проверки Account
class MockAccount: public Acount
{
	MOCK_METHOD(Account, ());
	MOCK_METHOD(GetBalance, (), (override));
	MOCK_METHOD(ChangeBalance, (int diff), (override));
	MOCK_METHOD(Lock, (), (override));
	MOCK_METHOD(Unlock, (), (override));
	MOCK_METHOD(~Account, (), (override));
}

class MockTransaction: public Transaction
{
	MOCK_METHOD(~Transaction, (), (override));
	MOCL_METHOD(Transaction, ());

  	bool Make(Account& from, Account& to, int sum);
  	int fee() const { return fee_; }
  	void set_fee(int fee) { fee_ = fee; }
}
