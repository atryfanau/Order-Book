#include "OrderBook.h"
#include "Enums.h"
#include "Order.h"
#include <list>
#include <chrono>
#include <ostream>
#include <iosfwd>
#include <fstream>

//Sets up the program
void OrderBook::openExchange()
{
	cout << "Enter simulation delay in milliseconds: ";
	cin >> delay;
	cout << "Enter the ticker for the stock: ";
	cin >> ticker;
	cout << "Enter the closing price from yesterday: ";
	cin >> closingPrice;
	cout << "Enter the order data file name: ";
	cin >> fileName;
	cout << endl;
}

//compares the other to the top of the queue
bool OrderBook::compareOrder(Order o)
{
	list<Order>& book = getOppBook(o);

	//checks to see that the opposite book has orders in it
	if (book.empty())
	{
		return false;
	}

	//checks to see whether a transaction can be made
	//returns true if <= askbook or >= bidbook
	if (o &= book.front().getPrice())
	{
		return true;
	}
	else
	{
		return false;
	}
}

//trades shares between object and list
void OrderBook::makeTransaction(Order& o)
{
	list<Order>& book = getOppBook(o);
	long int oShares = o.getShares();
	long int listShares = book.front().getShares();
	double tradePrice = book.front().getPrice();
	long int tradeShares;
	string sign;

	//cancels out the shares of both orders 
	o.setShares(oShares - listShares);
	book.front().setShares(listShares - oShares);

	//sets the amount of shares being traded
	tradeShares = listShares - book.front().getShares();

	//couts the transaction trace
	screenTrace(tradePrice, closingPrice);

	//outputs transaction to audit file
	writeAudit(book.front(), o, tradeShares, tradePrice);

	//cleans up empty book members and determines whether there's still
	//a transaction to be made
	if (book.front().getShares() == 0 && o.getShares() == 0)
	{
		book.pop_front();
		return;
	}
	else if (book.front().getShares() == 0)
	{
		book.pop_front();

		if (!book.empty() && compareOrder(o))
		{
			makeTransaction(o);
		}
	}
}

//returns an iterator to the proper position in proper book
void OrderBook::storeOrder(Order o)
{
	list<Order>::iterator it;
	list<Order>& book = getBook(o);

	if (book.empty())
	{
		book.emplace_front(o);
		return;
	}

	for (it = book.begin(); it != book.end(); ++it)
	{
		//returns true if < askbook or > bidbook
		if (o %= it->getPrice())
		{
			book.emplace(it, o);
			return;
		}
	}

	if (it == book.end())
	{
		book.emplace_back(o);
		return;
	}
}

//appends transaction to the audit file
void OrderBook::writeAudit(Order o1, Order o2, long int shares, double price)
{
	fstream file;
	Order b, s;

	if (o1.getAB() == BID)
	{
		b = o1;
		s = o2;
	}
	else
	{
		b = o2;
		s = o1;
	}

	file.open("audit.txt", fstream::app);

	using namespace std::chrono;
	uint64_t timeStamp = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()).count();

	file << b.getID() << " " << s.getID() << " " << price
		<< " " << shares << " " << timeStamp << endl;

	file.close();
}

//appends a market imbalance to the audit file
void OrderBook::writeAudit(Order o)
{
	fstream file;
	string oType;

	if (o.getAB() == BID)
	{
		oType = "Bid";
	}
	else if (o.getAB() == ASK)
	{
		oType = "Ask";
	}

	file.open("audit.txt", fstream::app);

	file << "Market Imbalance - " << oType << " order ID: "
		<< o.getID() << " Volume: " << o.getShares()
		<< " - unmatched" << endl;

	file.close();
}

//writes the book contents to appropriate files
void OrderBook::writeBooks()
{
	ofstream outputFile;
	list<Order>::iterator it;

	outputFile.open("askbook.txt");

	for (it = askbook.begin(); it != askbook.end(); it++)
	{
		outputFile << "1 " << "-1 " << it->getPrice()
			<< " " << it->getShares() << " " << it->getID()
			<< endl << endl;
	}

	outputFile.close();
	outputFile.open("bidbook.txt");

	for (it = bidbook.begin(); it != bidbook.end(); it++)
	{
		outputFile << "1 " << "1 " << it->getPrice()
			<< " " << it->getShares() << " " << it->getID()
			<< endl << endl;
	}

	outputFile.close();
}

//creates a line of screentrace
void OrderBook::screenTrace(double tp, double cp)
{
	string sign;
	string space = " ";
	int precision = 2;

	if (tp - cp >= 0)
	{
		sign = "+";
	}
	else
	{
		sign = "";
	}

	cout << ticker << space;
	cout << fixed << setprecision(precision) << tp << space;
	cout << sign << fixed << setprecision(precision) << tp - cp << space;
	cout << right << "(" << sign;
	cout << fixed << setprecision(precision) << ((tp - cp) / cp) * 100;
	cout << "%)" << endl;
}

//test function
void OrderBook::coutLists()
{
	list<Order>::iterator it;

	cout << "ASK BOOK:" << endl;
	for (it = askbook.begin(); it != askbook.end(); it++)
	{
		cout << it->getPrice() << "  " << it->getShares()
			<< "  " << it->getTimeStamp() << endl;
	}

	cout << "BID BOOK:" << endl;
	for (it = bidbook.begin(); it != bidbook.end(); it++)
	{
		cout << it->getPrice() << "  " << it->getShares()
			<< "  " << it->getTimeStamp() << endl;
	}

	cout << endl << endl;
}
