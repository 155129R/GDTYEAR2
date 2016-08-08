#ifndef CCARD_H
#define CCARD_H

#include <string>
using std::string;

class CCard
{
public:
    CCard();
    virtual ~CCard();

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

    string getName();

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