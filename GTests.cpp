#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "./banking/Account.h"
#include "./banking/Transaction.h"

//Добавить модули для проверки Account
class MockAccount: public Acount
{
	MockAccount(int id, int balance) : Account(id, balance) {};
	MOCK_METHOD(GetBalance, (), (ovverride));
	MOCK_METHOD(ChangeBalance, (int diff), (ovverride));
	MOCK_METHOD(Lock, (), (ovverride));
	MOCK_METHOD(Unlock, (), (ovverride));
	MOCK_METHOD(~Account, (), (ovverride));
}

class MockTransaction: public Transaction
{
	MOCK_METHOD(~Transaction, (), (ovverride));
}
