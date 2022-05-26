#include <third-party/gtest/gtest.h>

//Добавить модули для проверки Account

TEST(Banking, ChangeAndReturnBalance)
{
	Account temp(123, 0);
	temp.ChangeBalance(500000);
	int getBalance = temp.GetBalance();
	ASSERT_EQ(geBalance, 500000);
}

TEST(Banking, TryToChangeBalanceWhileLock)
{
	Accounr temp(1337, 2680);
}
