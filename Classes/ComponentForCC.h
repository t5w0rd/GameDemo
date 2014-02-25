#ifndef __COMPONENTFORCC_H__
#define __COMPONENTFORCC_H__


class CCProgressBar : public CCNode
{
public:
    static const float CONST_MAX_PROCESS_BAR_PERCENT;

public:
    CCProgressBar();
    virtual ~CCProgressBar();

    virtual bool init(const CCSize& roSize, CCSprite* pFill, CCSprite* pBorder, float fHorizBorderWidth, float fVertBorderWidth, bool bFillOnTop);
    M_CREATE_FUNC_PARAM(CCProgressBar, (const CCSize& roSize, CCSprite* pFill, CCSprite* pBorder, float fHorizBorderWidth, float fVertBorderWidth, bool bFillOnTop), roSize, pFill, pBorder, fHorizBorderWidth, fVertBorderWidth, bFillOnTop);

    virtual void setPercentage(float fPercent);
    virtual void setPercentage(float fPercent, float fDuration, CCFiniteTimeAction* pEndAction = NULL);
    virtual CCFiniteTimeAction* setPercentageAction(float fPercent, float fDuration, CCFiniteTimeAction* pEndAction = NULL);
    virtual void setFillColor(const ccColor3B& roColor);

public:
    CCProgressTimer* m_pPt;
};


#endif  /* __COMPONENTFORCC_H__ */
