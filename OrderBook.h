#pragma once

#include "Order.h"

class OrderBook
{
private:
	list<Order> askbook;
	list<Order> bidbook;
	int delay;
	string ticker;
	double closingPrice;
	string fileName;

public:
	//Sets up the program
	void openExchange();

	//compares the other to the top of the queue
	bool compareOrder(Order);

	//trades shares between object and list
	void makeTransaction(Order&);

	//stores an order inside the appropriate list
	void storeOrder(Order);

	//appends transaction or a market imbalance to the audit file
	void writeAudit(Order, Order, long int, double);
	void writeAudit(Order);

	//writes the book contents to appropriate files
	void writeBooks();

	//creates a line of screentrace
	void screenTrace(double, double);

	//test function
	void coutLists();

	//returns the book the object belongs to
	list<Order>& getBook(Order o)
	{
		if (o.getAB() == ASK)
		{
			return askbook;
		}
		else if (o.getAB() == BID)
		{
			return bidbook;
		}
	}

	//returns the book the object doesn't belong to
	list<Order>& getOppBook(Order o)
	{
		if (o.getAB() == ASK)
		{
			return bidbook;
		}
		else if (o.getAB() == BID)
		{
			return askbook;
		}
	}

	//GET FUNCTIONS
	int getDelay()
	{
		return delay;
	}
	string getTicker()
	{
		return ticker;
	}
	double getClosingPrice()
	{
		return closingPrice;
	}
	string getFileName()
	{
		return fileName;
	}
};
