#include "Enums.h"
#include "OrderBook.h"

void resetFiles();

int main()
{
	Order o;
	OrderBook bigBook;
	ifstream inputFile;

	//clears the output files
	resetFiles();

	//initializes the program
	bigBook.openExchange();

	//open the order file
	inputFile.open(bigBook.getFileName());

	//header for the screentrace
	cout << "SCREEN TRACE:" << endl;;

	if (inputFile)
	{
		while (true)
		{
			//delay
			Sleep(bigBook.getDelay());

			inputFile >> o;

			//determines whether there's a transaction to be made
			if (bigBook.compareOrder(o) && o.getShares() != 0)
			{
				bigBook.makeTransaction(o);
			}

			//determines whether the other should be written to one of
			//the books or whether there's an imbalance
			if (o.getType() == LIMITED && o.getShares() != 0)
			{
				bigBook.storeOrder(o);
			}
			else if (o.getType() == MARKET && o.getShares() != 0)
			{
				bigBook.writeAudit(o);
			}

			if (inputFile.eof())
			{
				break;
			}
		}

		//stores the unmatched orders
		bigBook.writeBooks();
	}
	else
	{
		cout << "ERROR OPENING FILE";
	}

	inputFile.close();

	cout << endl << endl << "Press any key to continue. . .";
	cin.ignore();
	cin.get();

	return 0;
}

//deletes all of the old files
void resetFiles()
{
	cout << "Cleaning up old files..." << endl;
	remove("audit.txt");
	remove("askbook.txt");
	remove("bidbook.txt");
	cout << "Old files removed successfully!" << endl << endl;
}
