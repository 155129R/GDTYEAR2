#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "CSVHandler.h"
#include "Card.h"

int main(void)
{
	CCSVHandler myCSVHandler;
	myCSVHandler.LoadFile("CR_Cards.csv");

	int count = 0;
	const int deckSize = myCSVHandler.cards.size();

	CCard* deck = new CCard[deckSize];
	for (int i = 0; i < deckSize; i++)
	{
		deck[i] = myCSVHandler.cards.at(i);
	}

	int input = 0;

	while (input != 4)
	{
		cout << "****************************" << endl;
		cout << "* Clash Royale Card System *" << endl;
		cout << "****************************" << endl;
		cout << "(1) Display ALL" << endl;
		cout << "(2) Sort Display" << endl;
		cout << "(3) Search" << endl;
		cout << "(4) Quit" << endl;

		cout << "Enter Choice: ";
		cin >> input;
		cout << endl;
		switch (input)
		{
		case 1:
		{
			cout << "<Display ALL>" << endl;
			CCard temp;
			for (int iter = 0; iter < deckSize; iter++)
			{
				cout << "#" << iter + 1 << ": " << deck[iter] << endl;
			}
			break;
		}
		case 2:
			break;
		case 3:
			break;
		default:
			break;
		}
		cout << endl;
		system("pause");
		system("cls");
	}
}