#include "CommHeader.h"

#include "ActionForCC.h"
#include "Logic/Draw.h"


// CCCallFuncNMultiObj
CCCallFuncNMultiObj::~CCCallFuncNMultiObj()
{
    if (m_pData != NULL)
    {
        ((CMultiRefObject*)m_pData)->release();
    }
}

bool CCCallFuncNMultiObj::initWithTarget( CCObject* pSelectorTarget, SEL_CallFuncND selector, CCallFuncData* d )
{
    if (d != NULL)
    {
        d->retain();
    }

    return CCCallFuncND::initWithTarget(pSelectorTarget, selector, d);
}

CCObject* CCCallFuncNMultiObj::copyWithZone( CCZone *zone )
{
    CCZone* pNewZone = NULL;
    CCCallFuncNMultiObj* pRet = NULL;

    if (zone && zone->m_pCopyObject) {
        //in case of being called at sub class
        pRet = (CCCallFuncNMultiObj*) (zone->m_pCopyObject);
    } else {
        pRet = new CCCallFuncNMultiObj();
        zone = pNewZone = new CCZone(pRet);
    }

    CCCallFunc::copyWithZone(zone);
    pRet->initWithTarget(m_pSelectorTarget, m_pCallFuncND, (CCallFuncData*)m_pData);
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

// CCNotifyAnimate
CCNotifyAnimate::~CCNotifyAnimate()
{
    if (m_pData != NULL)
    {
        m_pData->release();
    }
}

bool CCNotifyAnimate::initWithAnimation( CCAnimation* pAnimation, int iNotifyFrameIndex, CCObject* pSelector, SEL_CallFuncND pCallback, CCallFuncData* pData )
{
    if (!CCAnimate::initWithAnimation(pAnimation))
    {
        return false;
    }

    m_iNotifyFrameIndex = iNotifyFrameIndex;
    m_pSelector = pSelector;
    m_pCallback = pCallback;

    if (pData != NULL)
    {
        pData->retain();
    }
    m_pData = pData;
    
    return true;
}

void CCNotifyAnimate::update( float t )
{
    // if t==1, ignore. Animation should finish with t==1
    if( t < 1.0f ) {
        t *= getAnimation()->getLoops();

        // new loop?  If so, reset frame counter
        unsigned int loopNumber = (unsigned int)t;
        if( loopNumber > m_uExecutedLoops ) {
            m_nNextFrame = 0;
            m_uExecutedLoops++;
        }

        // new t for animations
        t = fmodf(t, 1.0f);
    }

    CCArray* frames = getAnimation()->getFrames();
    unsigned int numberOfFrames = frames->count();
    CCSpriteFrame *frameToDisplay = NULL;

    for( unsigned int i=m_nNextFrame; i < numberOfFrames; i++ ) {
        float splitTime = m_pSplitTimes->at(i);

        if( splitTime <= t ) {
            CCAnimationFrame* frame = (CCAnimationFrame*)frames->objectAtIndex(i);
            frameToDisplay = frame->getSpriteFrame();
            ((CCSprite*)m_pTarget)->setDisplayFrame(frameToDisplay);

            CCDictionary* dict = frame->getUserInfo();
            if( dict )
            {
                //TODO: [[NSNotificationCenter defaultCenter] postNotificationName:CCAnimationFrameDisplayedNotification object:target_ userInfo:dict];
            }
            m_nNextFrame = i+1;

            if (i == m_iNotifyFrameIndex && m_pSelector && m_pCallback)
            {
                (m_pSelector->*m_pCallback)(m_pTarget, m_pData);
            }

            break;
        }
    }
}

CCObject* CCNotifyAnimate::copyWithZone( CCZone* pZone )
{
    CCZone* pNewZone = NULL;
    CCNotifyAnimate* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCNotifyAnimate*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCNotifyAnimate();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithAnimation((CCAnimation*)getAnimation()->copy()->autorelease(), m_iNotifyFrameIndex, m_pSelector, m_pCallback, m_pData);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

// CCFadeInOutScale4
bool CCFadeInOutScale4::init( float fScaleStart, float fScaleMid, float fScaleEnd, float fDurToMid, float fDurToNormal, float fDurKeep, float fDurToEnd )
{
    m_fScaleStart = fScaleStart;
    CCActionInterval* pAct0 = CCSpawn::createWithTwoActions(CCFadeIn::create(fDurToMid), CCScaleTo::create(fDurToMid, fScaleMid));
    CCActionInterval* pAct1 = CCScaleTo::create(fDurToNormal, 1.0);
    CCActionInterval* pAct2 = CCScaleBy::create(fDurKeep, 1.0);
    CCActionInterval* pAct3 = CCSpawn::createWithTwoActions(CCFadeOut::create(fDurToEnd), CCScaleTo::create(fDurToEnd, fScaleEnd));

    return CCSequence::initWithTwoActions(CCSequence::create(pAct0, pAct1, pAct2, NULL), pAct3);
}

void CCFadeInOutScale4::startWithTarget( CCNode *pTarget )
{
    CCSequence::startWithTarget(pTarget);
    m_pTarget->setScale(m_fScaleStart);
}
