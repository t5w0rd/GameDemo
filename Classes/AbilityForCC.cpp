#include "CommHeader.h"

#include "Unit.h"
#include "Ability.h"
#include "AbilityForCC.h"
#include "ComponentForCC.h"
#include "DrawForCC.h"


// CStatusShowPas
CShowStatusPas::CShowStatusPas()
: CPassiveAbility("ShowStatusPas", "ShowStatus")
, m_pProgressBar(nullptr)
{
    setDbgClassName("CStatusShowPas");
    setTriggerFlags(CUnit::kOnChangeHpTrigger | CUnit::kOnReviveTrigger | CUnit::kOnDyingTrigger);
}

CShowStatusPas* CShowStatusPas::copy()
{
    return new CShowStatusPas();
}

void CShowStatusPas::onUnitAddAbility()
{
    CUnit* o = getOwner();
    CUnitDrawForCC* d = DCAST(o->getDraw(), CUnitDrawForCC*);
    assert(d != nullptr);
    UnitSprite* sp = d->getSprite();
    Node* sd = sp->getShadow();

    assert(m_pProgressBar == nullptr);
    float fBarWidth = d->getHalfOfWidth() * 2 + 20;
    float fBarPosHeight = d->getHalfOfHeight() * 2.5 + 10;
    m_pProgressBar = ProgressBar::create(Size(fBarWidth * sp->getScale(), 10.0f), Sprite::createWithSpriteFrameName("UI/healthbar/healthbar_fill.png"), Sprite::createWithSpriteFrameName("UI/healthbar/healthbar_border.png"), 1.0f, 1.0f, true);
    sd->addChild(m_pProgressBar);
    m_pProgressBar->setPosition(sd->getAnchorPointInPoints() + Point(0.0f, fBarPosHeight * sp->getScale()));
    onUnitChangeHp(0.0f);
}

void CShowStatusPas::onUnitDelAbility()
{
    assert(m_pProgressBar != nullptr);
    m_pProgressBar->removeFromParentAndCleanup(true);
    m_pProgressBar = nullptr;
}

void CShowStatusPas::onUnitChangeHp(float fChanged)
{
    CUnit* o = getOwner();
    float fPer = o->getHp() * 100 / max(FLT_EPSILON, o->getRealMaxHp());
    m_pProgressBar->setPercentage(fPer);
    m_pProgressBar->setFillColor(Color3B(min(255, (int)((100.0 - fPer) * 2.56 / 0.5)), min(255, (int)(2.56 / 0.5  * fPer)), 0));
}

void CShowStatusPas::onUnitRevive()
{
    m_pProgressBar->setVisible(true);
}

void CShowStatusPas::onUnitDying()
{
    m_pProgressBar->setVisible(false);
}
