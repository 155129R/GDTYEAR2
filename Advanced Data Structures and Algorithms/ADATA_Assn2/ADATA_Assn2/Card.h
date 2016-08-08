#ifndef CCARD_H
#define CCARD_H

#include <string>
#include <ostream>

using std::string;
using std::ostream;

class CCard
{
public:
	CCard();
	virtual ~CCard();

	CCard(string data);

	enum rarityValue
	{
		rV_COMMON,
		rV_RARE,
		rV_EPIC
	};

	enum cardType
	{
		cT_TROOP,
		cT_BUILDING,
		cT_SPELL
	};

	enum targetType
	{
		tT_GROUND,
		tT_AG,
		tT_BUILDINGS,
		tT_NA
	};

	string getCardName();
	int getElixirCost();
	rarityValue getRarityValue();
	cardType getCardType();
	targetType getTargetType();
	int getHitpoints();
	int getDamage();

	void setCardName(){}
	void setElixirCost(){}
	void setRarityValue(){}
	void setCardType(){}
	void setTargetType(){}
	void setHitpoints(){}
	void setDamage(){}

	friend ostream& operator<<(ostream& os, const CCard& card);

private:
	string m_strCardName;
	int m_iElixirCost;
	rarityValue m_rVRarity;
	cardType m_cTType;
	targetType m_tTTargets;
	int m_iHitpoints;
	int m_iDamage;

};

#endif /*CCARD_H*/