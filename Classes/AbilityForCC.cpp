#include "CommHeader.h"

#include "Unit.h"
#include "Ability.h"
#include "AbilityForCC.h"
#include "ComponentForCC.h"
#include "DrawForCC.h"


// CStatusShowPas
CStatusShowPas::CStatusShowPas()
    : CPassiveAbility("StatusShowPas", "ÑªÌõ")
    , m_pProgressBar(NULL)
{
    setDbgClassName("CStatusShowPas");
    setTriggerFlags(CUnit::kChangeHpTrigger);
}

CMultiRefObject* CStatusShowPas::copy() const
{
    return new CStatusShowPas();
}

void CStatusShowPas::onUnitAddAbility()
{
    CUnit* o = getOwner();
    CUnitDrawForCC* d = DCAST(o->getDraw(), CUnitDrawForCC*);
    assert(d != NULL);
    CCUnitSprite* sp = d->getSprite();
    CCNode* sd = sp->getShadow();

    assert(m_pProgressBar == NULL);
    CCSize sz(d->getHalfOfWidth() * 3, d->getHalfOfHeight() * 3);
    m_pProgressBar = CCProgressBar::create(CCSizeMake(sz.width * sp->getScale(), 6.0f), CCSprite::createWithSpriteFrameName("UI/healthbar_fill.png"), CCSprite::createWithSpriteFrameName("UI/healthbar_border.png"), 1.0f, 1.0f, true);
    sd->addChild(m_pProgressBar);
    m_pProgressBar->setPosition(ccpAdd(sd->getAnchorPointInPoints(), ccp(0.0f, sz.height * sp->getScale())));
    onUnitChangeHp(0.0f);
}

void CStatusShowPas::onUnitDelAbility()
{
    assert(m_pProgressBar != NULL);
    m_pProgressBar->removeFromParentAndCleanup(true);
    m_pProgressBar->release();
    m_pProgressBar = NULL;
}

void CStatusShowPas::onUnitChangeHp( float fChanged )
{
    CUnit* o = getOwner();
    float fPer = o->getHp() * 100 / max(FLT_EPSILON, o->getMaxHp());
    m_pProgressBar->setPercentage(fPer);
    m_pProgressBar->setFillColor(ccc3(min(255, (int)((100.0 - fPer) * 2.56 / 0.5)), min(255, (int)(2.56 / 0.5  * fPer)), 0));
}

