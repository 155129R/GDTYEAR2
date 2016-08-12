#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "CSVHandler.h"
#include "Card.h"

std::vector<CCard> resultantCards;

enum sortsearchType
{
    NAME,
    ELIXIRCOST,
    RARITY,
    TYPE,
    TARGETS,
    HITPOINTS,
    DAMAGE
};

void sortCards(sortsearchType sortSearchMethod, int length, bool ascend);
void searchCards(sortsearchType sortSearchMethod[]);

CCard* deck;
int deckLength;

int main(void)
{
	CCSVHandler myCSVHandler;
	myCSVHandler.LoadFile("CR_Cards.csv");

	int count = 0;
	const int deckSize = myCSVHandler.cards.size();

	deck = new CCard[deckSize];
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
			for (int iter = 0; iter < deckSize; iter++)
			{
				cout << "#" << iter + 1 << ": " << deck[iter] << endl;
			}
			break;
		}
        case 2:
        {
            cout << "<Sort Display>" << endl;
            cout << "Sort according to:" << endl;
            cout << "(1) Card Name" << endl;
            cout << "(2) Elixir Cost" << endl;
            cout << "(3) Rarity" << endl;
            cout << "(4) Type" << endl;
            cout << "(5) Targets" << endl;
            cout << "(6) Hitpoints" << endl;
            cout << "(7) Damage" << endl;
            int input;
            cout << "Enter Choice: ";
            cin >> input;
            sortsearchType ssInput;
            switch (input)
            {
            case 1: 
                ssInput = NAME;
                break;
            case 2:
                ssInput = ELIXIRCOST;
                break;
            case 3: 
                ssInput = RARITY;
                break;
            case 4:
                ssInput = TYPE;
                break;
            case 5:
                ssInput = TARGETS;
                break;
            case 6:
                ssInput = HITPOINTS;
                break;
            case 7:
                ssInput = DAMAGE;
                break;
            }
            cout << endl;
            cout << "Ascending (1) or Descending (2)" << endl;
            cout << "Enter Choice: ";
            cin >> input;
            cout << endl;
            sortCards(ssInput, deckSize, input - 1);
            break;
        }
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

void sortCards(sortsearchType sortSearchMethod, int length,bool ascend)
{
    switch (sortSearchMethod)
    {
    case NAME:
    {
        if (ascend)
        {
            for (int iter = 1; iter < length; iter++)
            {
                for (int index = 0; index < length - iter; index++)
                {
                    if (deck[index].getCardName() < deck[index + 1].getCardName())
                    {
                        //swap around
                        CCard temp = deck[index];
                        deck[index] = deck[index + 1];
                        deck[index + 1] = temp;
                    }
                }
            }
        }
        else
        {
            for (int iter = 1; iter < length; iter++)
            {
                for (int index = 0; index < length - iter; index++)
                {
                    if (deck[index].getCardName() > deck[index + 1].getCardName())
                    {
                        //swap around
                        CCard temp = deck[index];
                        deck[index] = deck[index + 1];
                        deck[index + 1] = temp;
                    }
                }
            }
        }
        break;
    }
    case ELIXIRCOST:
    {
        if (ascend)
        {
            for (int iter = 1; iter < length; iter++)
            {
                for (int index = 0; index < length - iter; index++)
                {
                    if (deck[index].getElixirCost() < deck[index + 1].getElixirCost())
                    {
                        //swap around
                        CCard temp = deck[index];
                        deck[index] = deck[index + 1];
                        deck[index + 1] = temp;
                    }
                }
            }
        }
        else
        {
            for (int iter = 1; iter < length; iter++)
            {
                for (int index = 0; index < length - iter; index++)
                {
                    if (deck[index].getElixirCost() > deck[index + 1].getElixirCost())
                    {
                        //swap around
                        CCard temp = deck[index];
                        deck[index] = deck[index + 1];
                        deck[index + 1] = temp;
                    }
                }
            }
        }
        break;
    }
    case RARITY:
    {
        if (ascend)
        {
            for (int iter = 1; iter < length; iter++)
            {
                for (int index = 0; index < length - iter; index++)
                {
                    if (deck[index].getRarityValue() < deck[index + 1].getRarityValue())
                    {
                        //swap around
                        CCard temp = deck[index];
                        deck[index] = deck[index + 1];
                        deck[index + 1] = temp;
                    }
                }
            }
        }
        else
        {
            for (int iter = 1; iter < length; iter++)
            {
                for (int index = 0; index < length - iter; index++)
                {
                    if (deck[index].getRarityValue() > deck[index + 1].getRarityValue())
                    {
                        //swap around
                        CCard temp = deck[index];
                        deck[index] = deck[index + 1];
                        deck[index + 1] = temp;
                    }
                }
            }
        }
        break;
    }
    case TYPE:
    {
        if (ascend)
        {
            for (int iter = 1; iter < length; iter++)
            {
                for (int index = 0; index < length - iter; index++)
                {
                    if (deck[index].getCardType() < deck[index + 1].getCardType())
                    {
                        //swap around
                        CCard temp = deck[index];
                        deck[index] = deck[index + 1];
                        deck[index + 1] = temp;
                    }
                }
            }
        }
        else
        {
            for (int iter = 1; iter < length; iter++)
            {
                for (int index = 0; index < length - iter; index++)
                {
                    if (deck[index].getCardType() > deck[index + 1].getCardType())
                    {
                        //swap around
                        CCard temp = deck[index];
                        deck[index] = deck[index + 1];
                        deck[index + 1] = temp;
                    }
                }
            }
        }
        break;
    }
    case TARGETS:
    {
        if (ascend)
        {
            for (int iter = 1; iter < length; iter++)
            {
                for (int index = 0; index < length - iter; index++)
                {
                    if (deck[index].getTargetType() < deck[index + 1].getTargetType())
                    {
                        //swap around
                        CCard temp = deck[index];
                        deck[index] = deck[index + 1];
                        deck[index + 1] = temp;
                    }
                }
            }
        }
        else
        {
            for (int iter = 1; iter < length; iter++)
            {
                for (int index = 0; index < length - iter; index++)
                {
                    if (deck[index].getTargetType() > deck[index + 1].getTargetType())
                    {
                        //swap around
                        CCard temp = deck[index];
                        deck[index] = deck[index + 1];
                        deck[index + 1] = temp;
                    }
                }
            }
        }
        break;
    }
    case HITPOINTS:
    {
        if (ascend)
        {
            for (int iter = 1; iter < length; iter++)
            {
                for (int index = 0; index < length - iter; index++)
                {
                    if (deck[index].getHitpoints() < deck[index + 1].getHitpoints())
                    {
                        //swap around
                        CCard temp = deck[index];
                        deck[index] = deck[index + 1];
                        deck[index + 1] = temp;
                    }
                }
            }
        }
        else
        {
            for (int iter = 1; iter < length; iter++)
            {
                for (int index = 0; index < length - iter; index++)
                {
                    if (deck[index].getHitpoints() > deck[index + 1].getHitpoints())
                    {
                        //swap around
                        CCard temp = deck[index];
                        deck[index] = deck[index + 1];
                        deck[index + 1] = temp;
                    }
                }
            }
        }
        break;
    }
    case DAMAGE:
    {
        if (ascend)
        {
            for (int iter = 1; iter < length; iter++)
            {
                for (int index = 0; index < length - iter; index++)
                {
                    if (deck[index].getDamage() < deck[index + 1].getDamage())
                    {
                        //swap around
                        CCard temp = deck[index];
                        deck[index] = deck[index + 1];
                        deck[index + 1] = temp;
                    }
                }
            }
        }
        else
        {
            for (int iter = 1; iter < length; iter++)
            {
                for (int index = 0; index < length - iter; index++)
                {
                    if (deck[index].getDamage() > deck[index + 1].getDamage())
                    {
                        //swap around
                        CCard temp = deck[index];
                        deck[index] = deck[index + 1];
                        deck[index + 1] = temp;
                    }
                }
            }
        }
        break;
    }
    }
}

void searchCards(std::vector<sortsearchType, int> search_Criteria)
{
    for (auto criteria : search_Criteria)
    {
        if (criteria == NAME)
        {
            for (int iter = 0; iter < deckLength; iter++)
            {
                if (deck[iter].getCardName() == criteria)
                {
                    resultantCards.push_back(deck[iter]);
                }
            }
        }
    }
}