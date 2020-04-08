#include "Order.h"
#include <chrono>
#include <ostream>
#include <iosfwd>
#include <fstream>

void Order::initAB(int b)
{
	if (b == 1)
	{
		ab = BID;
	}
	else if (b == -1)
	{
		ab = ASK;
	}
	else
	{
		cout << "Error parsing AB. test = " + b;
	}
}

void Order::initType(int t)
{
	if (t == 1)
	{
		type = LIMITED;
	}
	else if (t == 0)
	{
		type = MARKET;
	}
	else
	{
		cout << "Error parsing type. test = " + t;
	}
}

istream& operator>>(istream& is, Order& o)
{
	int test;

	is >> test;
	o.initType(test);
	is >> test;
	o.initAB(test);
	is >> o.price >> o.shares >> o.accountID;

	using namespace std::chrono;
	o.timeStamp = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()).count();

	return is;
}

ostream& operator<<(ostream& os, Order& o)
{
	os << o.price;
	return os;
}
