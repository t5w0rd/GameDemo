#include "CommHeader.h"

#include "Unit.h"
#include "Ability.h"
#include "AbilityForCC.h"
#include "ComponentForCC.h"
#include "DrawForCC.h"


// CStatusShowPas
const float CShowStatusPas::CONST_INTERVAL = 0.2f;
const float CShowStatusPas::CONST_DPS_INTERVAL = 2.0f;

CShowStatusPas::CShowStatusPas()
: CPassiveAbility("ShowStatusPas", "ShowStatus")
, m_pHpBar(nullptr)
, m_pDps(nullptr)
, m_fTotalDamage(0.0f)
, m_fDpsDuration(0.0f)
, m_fDpsUpdate(0.0f)
, m_f0DpsDuration(0.0f)
{
    setDbgClassName("CStatusShowPas");
    setTriggerFlags(CUnit::kOnChangeHpTrigger | CUnit::kOnReviveTrigger | CUnit::kOnDyingTrigger | CUnit::kOnCalcDamageTargetTrigger);
    setInterval(CONST_INTERVAL);
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

    assert(m_pHpBar == nullptr);
    float fBarWidth = d->getHalfOfWidth() * 2 + 20;
    float fBarPosHeight = d->getHalfOfHeight() * 2.5 + 10;
    m_pHpBar = ProgressBar::create(Size(fBarWidth * sp->getScale(), 10.0f), Sprite::createWithSpriteFrameName("UI/healthbar/healthbar_fill.png"), Sprite::createWithSpriteFrameName("UI/healthbar/healthbar_border.png"), 1.0f, 1.0f, true);
    sd->addChild(m_pHpBar);
    m_pHpBar->setPosition(sd->getAnchorPointInPoints() + Point(0.0f, fBarPosHeight * sp->getScale()));
    onUnitChangeHp(0.0f);

    m_fTotalDamage = 0.0f;
    m_fDpsDuration = 0.0f;
    m_fDpsUpdate = 0.0f;
    m_f0DpsDuration = 0.0f;
    m_pDps = Label::createWithBMFont("fonts/SohoGothicProMedium.fnt", "DPS: 0");
    sd->addChild(m_pDps);
    m_pDps->setPosition(m_pHpBar->getPosition() + Point(0.0f, m_pHpBar->getContentSize().height * 0.5 + m_pDps->getContentSize().height * 0.5));
    m_pDps->setVisible(false);
    m_pDps->setScale(0.8f);
    m_pDps->setColor(Color3B(255, 100, 100));

}

void CShowStatusPas::onUnitDelAbility()
{
    assert(m_pHpBar != nullptr);
    m_pHpBar->removeFromParentAndCleanup(true);
    m_pHpBar = nullptr;
    
    assert(m_pDps != nullptr);
    m_pDps->removeFromParentAndCleanup(true);
    m_pDps = nullptr;
}

void CShowStatusPas::onUnitChangeHp(float fChanged)
{
    CUnit* o = getOwner();
    float fPer = o->getHp() * 100 / max(FLT_EPSILON, o->getRealMaxHp());
    m_pHpBar->setPercentage(fPer);
    m_pHpBar->setFillColor(Color3B(min(255, (int)((100.0 - fPer) * 2.56 / 0.5)), min(255, (int)(2.56 / 0.5  * fPer)), 0));
}

void CShowStatusPas::onUnitRevive()
{
    m_pHpBar->setVisible(true);
}

void CShowStatusPas::onUnitDying()
{
    m_pHpBar->setVisible(false);
    m_pDps->setVisible(false);
}
// DPS: 停止输出伤害持续 CONST_DPS_INTERVAL 秒后，重置统计
void CShowStatusPas::onUnitInterval()
{
    if (m_fTotalDamage == 0.0f)
    {
        return;
    }

    m_f0DpsDuration += getInterval();
    m_fDpsDuration += getInterval();
    

    if (m_f0DpsDuration > CONST_DPS_INTERVAL)
    {
        // 没有持续输出，将重置统计
        m_fTotalDamage = 0.0f;
        m_fDpsDuration = 0.0f;
        m_fDpsUpdate = 0.0f;
        m_pDps->setVisible(false);
    }
    else if (m_fDpsDuration > CONST_DPS_INTERVAL)
    {
        m_fDpsUpdate += getInterval();
        if (m_fDpsUpdate >= CONST_DPS_INTERVAL)
        {
            m_fDpsUpdate = 0.0f;
            // 更新统计
            int dps = toInt(m_fTotalDamage / m_fDpsDuration);
            if (dps > 0)
            {
                char sz[64];
                sprintf(sz, "DPS: %d", dps);
                m_pDps->setString(sz);
                m_pDps->setVisible(true);
                m_f0DpsDuration = 0.0f;
            }
            else
            {
                m_pDps->setVisible(false);
            }
        }
    }
}

void CShowStatusPas::onUnitCalcDamageTarget(float fDamage, CUnit* pTarget)
{
    if (m_fTotalDamage == 0.0f)
    {
        m_fDpsUpdate = CONST_DPS_INTERVAL;
    }
    m_fTotalDamage += fDamage;
    m_f0DpsDuration = 0.0f;
}

