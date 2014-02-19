/* 
 * File:   UnitDraw.h
 * Author: thunderliu
 *
 * Created on 2014年2月13日, 下午6:34
 */

#ifndef __UNITDRAW_H__
#define	__UNITDRAW_H__

#include "Base.h"
#include "MultiRefObject.h"
#include "Unit.h"


class CCallFuncData : public CMultiRefObject
{
public:
    CCallFuncData(CMultiRefObject* pSel, FUNC_CALLFUNC_ND pCallFunc);

    M_SYNTHESIZE(CMultiRefObject*, m_pSelector, Selector);
    M_SYNTHESIZE(FUNC_CALLFUNC_ND, m_pCallFunc, CallFunc);
};

class CUnitDraw : public CMultiRefObject
{
public:
    CUnitDraw(const char* pName);
    virtual ~CUnitDraw();

    M_SYNTHESIZE_STR(Name);

    M_SYNTHESIZE(CUnit*, m_pUnit, Unit);

    enum ANI_ID
    {
        kActMove = 0x2000,
        kActDie,
        kAct1,
        kAct2,
        kAct3,
        kAct4,
        kAct5,
        kAct6,
        kAct7,
        kAct8
    };

    // 返回actionTag
    virtual int doAnimation(ANI_ID id, CCallFuncData* pOnNotifyFrame, int iRepeatTimes, CCallFuncData* pOnAnimationDone, float fSpeed = 1.0f);
    virtual void stopAction(int tag);
    virtual void setActionSpeed(int tag, float fSpeed);
    virtual bool isDoingAction(int id);
    virtual void stopAllActions();

    enum FRM_ID
    {
        kFrmDefault
    };
    
    virtual void setFrame(FRM_ID id);
    virtual void setFlipX(bool bFlipX);
    
    virtual void loadAnimation(int id, const char* pName, int iNotifyFrameIndex, float fDelay);
    virtual void loadFrame(int id, const char* pName);
};

class CUnitDraw2D : public CUnitDraw
{
public:
    CUnitDraw2D(const char* pName);
    virtual ~CUnitDraw2D();

    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oPosition, Position);

    M_SYNTHESIZE(float, m_fHalfOfWidth, HalfOfWidth);
    M_SYNTHESIZE(float, m_fHalfOfHeight, HalfOfHeight);

    /////////////////////// move //////////////////////////////
    virtual int doMoveTo(const CPoint& rPos, float fDuration, CCallFuncData* pOnMoveToDone, float fSpeed = 1.0f);
    
    virtual void setFlipX(bool bFlipX = true);
    virtual bool isFlipX() const;

    struct UNIT_MOVE_PARAMS
    {
        UNIT_MOVE_PARAMS(
            bool bIntended_ = true,
            bool bAutoFlipX_ = true,
            float fMaxOffsetY_ = 0.0f,
            bool bCancelCast_ = true
            )
            : bIntended(bIntended_)
            , bAutoFlipX(bAutoFlipX_)
            , fMaxOffsetY(fMaxOffsetY_)
            , bCancelCast(bCancelCast_)
        {}
        bool bIntended;
        bool bAutoFlipX;
        float fMaxOffsetY;
        bool bCancelCast;
    };

    static const UNIT_MOVE_PARAMS CONST_DEFAULT_MOVE_PARAMS;
    static const float CONST_MIN_MOVE_SPEED;
    static const float CONST_MAX_MOVE_SPEED;
    static const float CONST_MIN_MOVE_SPEED_MULRIPLE;

    M_SYNTHESIZE_READONLY(float, m_fBaseMoveSpeed, BaseMoveSpeed);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(CExtraCoeff, m_oExMoveSpeed, ExMoveSpeed);
    M_SYNTHESIZE_BOOL(Fixed);
    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oMoveTarget, MoveTarget);

    void setBaseMoveSpeed(float fMoveSpeed);
    void setExMoveSpeed(const CExtraCoeff& roExMoveSpeed);

    float getRealMoveSpeed() const;

    virtual void updateMoveSpeedDelta();


    M_SYNTHESIZE(int, m_iMoveToActionId, MoveToActionId);
    M_SYNTHESIZE(int, m_iMoveActionId, MoveActionId);
    void move(const CPoint& roPos, const UNIT_MOVE_PARAMS& roMoveParams = CONST_DEFAULT_MOVE_PARAMS);
    void follow(int iTargetUnit, const UNIT_MOVE_PARAMS& roMoveParams = CONST_DEFAULT_MOVE_PARAMS);
    //virtual void moveAlongPath(CUnitPath* pPath, bool bIntended = true, bool bRestart = false, float fBufArrive = 5.0);
    void stopMove();
    void onMoveDone(CMultiRefObject* pUnit, CCallFuncData* pData);

    void startMoveAct(const CPoint& roPos, const UNIT_MOVE_PARAMS& roMoveParams = CONST_DEFAULT_MOVE_PARAMS);
    void stopMoveAct();

    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oLastMoveToTarget, LastMoveToTarget);
    //M_SYNTHESIZE(uint32_t, m_dwPathCurPos, PathCurPos);
    //virtual void setPathIntended(bool bPathIntended = true);
    //virtual bool isPathIntended() const;

    
    // /////////////////////// cast /////////////////////////////
    M_SYNTHESIZE_PASS_BY_REF(CCommandTarget, m_oCastTarget, CastTarget);
    int cmdCastSpell(int iActiveSkillId);  // 可能是施法失败，施法中，施法追逐中，所以返回类型为int
    int castSpell(CActiveSkill* pSkill);
    bool checkCastTargetDistance(CActiveSkill* pSkill, const CPoint& roPos, CUnitDraw2D* td);
    void moveToCastPosition(CActiveSkill* pSkill, CUnitDraw2D* td);
};

#endif	/* __UNITDRAW_H__ */

