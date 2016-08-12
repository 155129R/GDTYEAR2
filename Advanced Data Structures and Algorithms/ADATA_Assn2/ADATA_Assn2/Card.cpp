#include "Card.h"
#include <iostream>

CCard::CCard()
	:m_cTType(cT_BUILDING)
	, m_iDamage(0)
	, m_iElixirCost(0)
	, m_iHitpoints(0)
	, m_rVRarity(rV_COMMON)
	, m_strCardName(" ")
	, m_tTTargets(tT_AG)
{
}


CCard::~CCard()
{
}

CCard::CCard(string data)
{
	//std::cout << data << std::endl;
	string::iterator commaLoc;
	string temp;

	for (string::iterator it = data.begin(); it < data.end(); ++it)
	{
		if (*it != ',')
			this->m_strCardName += *it;
		else
		{
			commaLoc = it;
			break;
		}
	}
	
	temp = "";
	for (string::iterator it = commaLoc + 1; it < data.end(); ++it)
	{
		if (*it != ',')
			temp += *it;
		else
		{
			commaLoc = it;
			break;
		}
	}
	this->m_iElixirCost = std::stoi(temp);
	temp = "";
	for (string::iterator it = commaLoc + 1; it < data.end(); ++it)
	{
		if (*it != ',')
		{
			temp += *it;	
		}
		else
		{
			commaLoc = it;
			break;
		}
	}
	if (temp == "Common")
		this->m_rVRarity = rV_COMMON;

	if (temp == "Rare")
		this->m_rVRarity = rV_RARE;

	if (temp == "Epic")
		this->m_rVRarity = rV_EPIC;

	temp = "";
	for (string::iterator it = commaLoc + 1; it < data.end(); ++it)
	{
		if (*it != ',')
		{
			temp += *it;
		}
		else
		{
			commaLoc = it;
			break;
		}
	}
	if (temp == "Troop")
		this->m_cTType = cT_TROOP;
	if (temp == "Spell")
		this->m_cTType = cT_SPELL;
    if (temp == "Building")
		this->m_cTType = cT_BUILDING;

	temp = "";
	for (string::iterator it = commaLoc + 1; it < data.end(); ++it)
	{
		if (*it != ',')
		{
			temp += *it;
		}
		else
		{
			commaLoc = it;
			break;
		}
	}

	if (temp == "Ground")
		this->m_tTTargets = tT_GROUND;
	if (temp == "Buidings")
		this->m_tTTargets = tT_BUILDINGS;
	if (temp == "AG")
		this->m_tTTargets = tT_AG;
	if (temp == "NA")
		this->m_tTTargets = tT_NA;
	temp = "";
	for (string::iterator it = commaLoc + 1; it < data.end(); ++it)
	{
		if (*it != ',')
		{
			temp += *it;
		}
		else
		{
			commaLoc = it;
			break;
		}
	}
	this->m_iHitpoints = std::stoi(temp);
	temp = "";

	for (string::iterator it = commaLoc + 1; it < data.end(); ++it)
	{
		if (*it != ',')
		{
			temp += *it;
		}
		else
		{
			commaLoc = it;
			break;
		}
	}
	this->m_iDamage = std::stoi(temp);
	temp = "";
}

ostream& operator<<(ostream& os, CCard& card)
{
	os << card.getCardName() << ",    " << card.getElixirCost() << ",    ";
	switch (card.getRarityValue())
	{
	case 0: os << "Common";
		break;
	case 1: os << "Rare";
		break;
	case 2: os << "Epic";
		break;
	}
	os << ",    ";
	switch (card.getCardType())
	{
	case 0: os << "Troop";
		break;
	case 1: os << "Building";
		break;
	case 2: os << "Spell";
		break;
	}
	os << ",    ";
	switch (card.getTargetType())
	{
	case 0: os << "Ground";
		break;
	case 1: os << "AG";
		break;
	case 2: os << "Buildings";
		break;
	case 3: os << "NA";
		break;
	}
	os << ",    " << card.getHitpoints() << ",    " << card.getDamage();
	return os;
}

string CCard::getCardName()
{
	return this->m_strCardName;
}
int CCard::getElixirCost()
{
	return this->m_iElixirCost;
}
CCard::rarityValue CCard::getRarityValue()
{
	return this->m_rVRarity;
}
CCard::cardType CCard::getCardType()
{
	return this->m_cTType;
}
CCard::targetType CCard::getTargetType()
{
	return this->m_tTTargets;
}
int CCard::getHitpoints()
{
	return this->m_iHitpoints;
}
int CCard::getDamage()
{
	return this->m_iDamage;
}