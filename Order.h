#pragma once
#include "Enums.h"

class Order
{
private:
	ORDER_TYPE type;
	ORDER_AB ab;
	double price;
	long int shares;
	long int accountID;
	uint64_t timeStamp;

public:
	//default constructor
	Order()
	{
	}
	//copy constructor
	Order(const Order& o)
	{
		type = o.type;
		ab = o.ab;
		price = o.price;
		shares = o.shares;
		accountID = o.accountID;
		timeStamp = o.timeStamp;
	}

	//methods for setting order variables
	void setLimited(ORDER_TYPE o)
	{
		type = o;
	}
	void setBuy(ORDER_AB o)
	{
		ab = o;
	}
	void setPrice(double p)
	{
		price = p;
	}
	void setShares(long int s)
	{
		//can't have negative shares. makeTransaction() is built on this
		if (s < 0)
		{
			s = 0;
		}

		shares = s;
	}
	void setID(long int i)
	{
		accountID = i;
	}
	void setTimeStamp(uint64_t t)
	{
		timeStamp = t;
	}

	//functions that parse stdin
	void initType(int);
	void initAB(int);

	//methods for getting order variables
	ORDER_TYPE getType()
	{
		return type;
	}
	ORDER_AB getAB()
	{
		return ab;
	}
	double getPrice()
	{
		return price;
	}
	long int getShares()
	{
		return shares;
	}
	long int getID()
	{
		return accountID;
	}
	uint64_t getTimeStamp()
	{
		return timeStamp;
	}

	//comparison overloads
	bool operator>(const Order& right)
	{
		return price > right.price;
	}
	bool operator>=(const Order& right)
	{
		return price >= right.price;
	}
	bool operator<(const Order& right)
	{
		return price < right.price;
	}
	bool operator<=(const Order& right)
	{
		return price <= right.price;
	}
	bool operator==(const Order& right)
	{
		return price == right.price;
	}

	//special comparisons
	bool operator%=(const double& right)
	{
		//we know the book is not empty, so a market order can
		//always make a trade
		if (type == MARKET)
		{
			return true;
		}
		else if (ab == ASK)
		{
			return price < right;
		}
		else if (ab == BID)
		{
			return price > right;
		}
	}

	bool operator&=(const double& right)
	{
		//we know the book is not empty, so a market order can
		//always make a trade
		if (type == MARKET)
		{
			return true;
		}
		else if (ab == ASK)
		{
			return price <= right;
		}
		else if (ab == BID)
		{
			return price >= right;
		}
	}

	//io operator overloads
	friend istream& operator>>(istream& is, Order& o);
	friend ostream& operator<<(ostream& os, Order& o);
};
