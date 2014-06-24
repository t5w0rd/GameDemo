/* 
 * File:   UnitDraw.h
 * Author: thunderliu
 *
 * Created on 2014年2月13日, 下午6:34
 */

#ifndef __DRAW_H__
#define	__DRAW_H__

#include "Base.h"
#include "MultiRefObject.h"
#include "Unit.h"


class CUnitDraw : public CMultiRefObject
{
public:
    CUnitDraw();
    virtual ~CUnitDraw();
    virtual CUnitDraw* copy() override;
    virtual void copyData(CUnitDraw* from);

    M_SYNTHESIZE(CUnit*, m_pUnit, Unit);

    // 返回actionTag
    virtual int doAnimation(int id, const FUNC_VOID& onNotifyFrame, int iRepeatTimes, const FUNC_VOID& onAnimationDone, float fSpeed = 1.0f);
    virtual void stopAction(int tag);
    virtual void setActionSpeed(int tag, float fSpeed);
    virtual bool isDoingAction(int id);
    virtual void stopAllActions();
    
    virtual void setVisible(bool bVisible = true);

    virtual void onUnitRevive();
    virtual void onUnitDying();
    virtual void onUnitTick(float dt);

    virtual void setFrame(int id);
    virtual void setFrameByAnimation(int id, int index);
    virtual void setFlippedX(bool bFlipX);
    
    virtual void loadAnimation(int id, const char* pName, int iNotifyFrameIndex, float fDelay);
    virtual void loadFrame(int id, const char* pName);

    virtual void say(const char* words);
};

class CUnitPath;
class CUnitDraw2D : public CUnitDraw
{
public:
    CUnitDraw2D();
    virtual ~CUnitDraw2D();
    virtual CUnitDraw2D* copy() override;
    virtual void copyData(CUnitDraw* from);

    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oPosition, Position);

    M_SYNTHESIZE(float, m_fHalfOfWidth, HalfOfWidth);
    M_SYNTHESIZE(float, m_fHalfOfHeight, HalfOfHeight);
    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oFirePoint, FirePoint);

    virtual void onUnitRevive();
    virtual void onUnitDying();
    virtual void onUnitTick(float dt);

    M_SYNTHESIZE(float, m_fHeight, Height);

    /////////////////////// move //////////////////////////////
    virtual int doMoveTo(const CPoint& rPos, float fDuration, const FUNC_VOID& onMoveToDone, float fSpeed = 1.0f);
    virtual void updateMoveTo(const CPoint& rPos);
    
    virtual void setFlippedX(bool bFlipX = true);
    virtual bool isFlippedX() const;

    struct UNIT_MOVE_PARAMS
    {
        UNIT_MOVE_PARAMS(
            //bool bObstinate_ = true,
            bool bAutoFlipX_ = true,
            float fMaxOffsetY_ = 0.0f
            //bool bCancelCast_ = true
          )
            //: bObstinate(bObstinate_)
        : bAutoFlipX(bAutoFlipX_)
        , fMaxOffsetY(fMaxOffsetY_)
            //, bCancelCast(bCancelCast_)
        {}
        //bool bObstinate;
        bool bAutoFlipX;
        float fMaxOffsetY;
        //bool bCancelCast;
    };

    static const UNIT_MOVE_PARAMS CONST_DEFAULT_MOVE_PARAMS;
    static const float CONST_MIN_MOVE_SPEED;
    static const float CONST_MAX_MOVE_SPEED;
    static const float CONST_MIN_MOVE_SPEED_MULRIPLE;

    M_SYNTHESIZE_READONLY(float, m_fBaseMoveSpeed, BaseMoveSpeed);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(CExtraCoeff, m_oExMoveSpeed, ExMoveSpeed);
    M_SYNTHESIZE_BOOL(Fixed);
    M_SYNTHESIZE(float, m_fHostilityRange, HostilityRange);
    //M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oMoveTarget, MoveTarget);

    void setBaseMoveSpeed(float fMoveSpeed);
    void setExMoveSpeed(const CExtraCoeff& roExMoveSpeed);

    float getRealMoveSpeed() const;

    void updateMoveSpeedDelta();


    M_SYNTHESIZE(int, m_iMoveToActionId, MoveToActionId);
    M_SYNTHESIZE(int, m_iMoveActionId, MoveActionId);
    void cmdMove(const CPoint& roPos, bool bObstinate = true);
    void move(const CPoint& roPos);  //, const UNIT_MOVE_PARAMS& roMoveParams = CONST_DEFAULT_MOVE_PARAMS);
    void follow(int iTargetUnit);  //, const UNIT_MOVE_PARAMS& roMoveParams = CONST_DEFAULT_MOVE_PARAMS);
    void cmdMoveAlongPath(CUnitPath* pPath, bool bObstinate = true, float fBufArrive = 5.0);
    void stopMove();
    void onMoveDone();

    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oLastMoveToTarget, LastMoveToTarget);
    M_SYNTHESIZE_READONLY(CUnitPath*, m_pMovePath, MovePath);
    M_SYNTHESIZE(uint32_t, m_dwPathCurPos, PathCurPos);
    M_SYNTHESIZE(float, m_fPathBufArrive, PathBufArrive);

protected:
    bool m_bPathObstinate;

public:
    // /////////////////////// cast /////////////////////////////
    M_SYNTHESIZE_PASS_BY_REF(CCommandTarget, m_oCastTarget, CastTarget);
    M_SYNTHESIZE(int, m_iCastActionId, CastActionId);
    M_SYNTHESIZE(int, m_iCastActiveAbilityId, CastActiveAbilityId);  // 正在进行的施法，包括追逐状态中的技能
    M_SYNTHESIZE(int, m_iWaitForCastTargetActiveAbilityId, WaitForCastTargetActiveAbilityId);  // 等待目标的技能
    int cmdCastSpell(const CCommandTarget& rTarget, int iActiveAbilityId, bool bObstinate = true);  // 可能是施法失败，施法中，施法追逐中，所以返回类型为int
    int castSpell(CActiveAbility* pAbility);
    bool checkCastTargetDistance(CActiveAbility* pAbility, const CPoint& roPos, const CCommandTarget& rTarget, CUnitDraw2D* td /* for fast calc */);
    void moveToCastPosition(CActiveAbility* pAbility, CUnitDraw2D* td);

    void stopCast(bool bDefaultFrame = true);
    void onCastEffect();
    void onCastDone();

    void cmdStop();
    void stop(bool bDefaultFrame = true);

    void die();
    void onDyingDone();
};

class CUnitPath : public CMultiRefObject
{
public:
    typedef vector<CPoint> VEC_POINTS;

public:
    CUnitPath();
    CUnitPath(const VEC_POINTS& roVecPoints);

    void addPoint(const CPoint& roPos);
    const CPoint* getCurTargetPoint(uint32_t dwCurPos);
    bool arriveCurTargetPoint(uint32_t& rCurPos);  // return true when end

public:
    VEC_POINTS m_vecPoints;
};

class CUnitGroup : public CMultiRefObject
{
public:
    typedef CMultiRefVec<CUnit*> VEC_UNITS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_UNITS, m_vecUnits, Units);

    typedef function<bool(CUnit* pUnit)> FUNC_MATCH;

public:
    CUnitGroup();
    CUnitGroup(CWorld* pWorld, const CPoint& roPos, float fRadius, int iMaxCount = INFINITE, const FUNC_MATCH& match = nullptr);
    CUnitGroup(CWorld* pWorld, int iMaxCount = INFINITE, const FUNC_MATCH& match = nullptr);

    CUnit* getUnitByIndex(int iIndex);
    CUnit* getRandomUnit();
    CUnit* getNearestUnitInRange(const CPoint& roPos, float fRadius, const FUNC_MATCH& match = nullptr);
    void addUnit(CUnit* pUnit);

    static CUnit* getNearestUnitInRange(CWorld* pWorld, const CPoint& roPos, float fRadius, const FUNC_MATCH& match = nullptr);

    void cleanUnits();
    int getUnitsCount();

    void damaged(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask = CUnit::kNoMasked);

    //virtual void setRangePosition(const CPoint& roPos, float fRadius);
    //virtual void turnTo(bool bLeft);
    //virtual void move(const CPoint& roPos, const CUnit::UNIT_MOVE_PARAMS& roMoveParams = CUnit::CONST_DEFAULT_MOVE_PARAMS);
    //virtual void followTo(int iTargetKey, const CUnit::UNIT_MOVE_PARAMS& roMoveParams = CUnit::CONST_DEFAULT_MOVE_PARAMS);
    //virtual void stopMove();
    //virtual void attack(int iTargetKey, bool bObstinate = true);
    //virtual void stopAttack();
    //virtual void moveAlongPath(CUnitPath* pPath, bool bObstinate = true, bool bRestart = false, float fBufArrive = 5.0);

    //void damagedAdv(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask = CUnit::kNoMasked);
    //void damagedMid(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask = CUnit::kNoMasked);
    //void damagedBot(float fDamage, CUnit* pSource, uint32_t dwTriggerMask = CUnit::kNoMasked);
    //void addActiveAbility(CAbility* pAbility);
    //void addBuff(CBuffAbility* pBuff, bool bForce = false);

    static FUNC_MATCH matchLivingAlly(CUnitForce* ref);
    static FUNC_MATCH matchLivingEnemy(CUnitForce* ref);
    static FUNC_MATCH matchLivingAlly(CUnit* ref);
    static FUNC_MATCH matchLivingEnemy(CUnit* ref);

protected:
    static bool funcMatchLivingAlly(CUnit* pUnit, CUnitForce* pParam);
    static bool funcMatchLivingEnemy(CUnit* pUnit, CUnitForce* pParam);

};

class CProjectile : public CMultiRefObject
{
public:
    CProjectile();
    virtual ~CProjectile();
    virtual CMultiRefObject* copy();
    virtual void copyData(const CProjectile* from);

    M_SYNTHESIZE(CWorld*, m_pWorld, World);

    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oPosition, Position);
    M_SYNTHESIZE(float, m_fHeight, Height);

    M_SYNTHESIZE(float, m_fMoveSpeed, MoveSpeed);
    M_SYNTHESIZE(float, m_fMaxHeightDelta, MaxHeightDelta);
    virtual float getRadius() const;

    enum ANI_ID
    {
        kAniMove,
        kAniDie
    };

    virtual int doLinkUnitToUnit(CUnit* pFromUnit, CUnit* pToUnit, ANI_ID id, const FUNC_VOID& onNotifyFrame, int iRepeatTimes, const FUNC_VOID& onAnimationDone);
    virtual int doMoveToUnit(CUnit* pToUnit, bool bFixRotation, float fMaxHeightDelta, float fDuration, const FUNC_VOID& onMoveToDone);
    virtual int doMoveTo(const CPoint& rPos, float fDuration, const FUNC_VOID& onMoveToDone);
    virtual int doAnimation(int id, const FUNC_VOID& onNotifyFrame, int iRepeatTimes, const FUNC_VOID& onAnimationDone);
    virtual void stopAction(int tag);
    virtual bool isDoingAction(int id);
    virtual void stopAllActions();

    virtual void setVisible(bool bVisible = true);

    enum FRM_ID
    {
        kFrmDefault
    };

    void die();
    void effect(CUnit* pTarget);

    virtual void step(float dt);
    virtual void onTick(float dt);
    void onMoveDone();
    void onEffect();
    void onDyingDone();

    // 单位和抛射物非紧密联系，即单位死亡后抛射物不一定会释放，所以必须通过ID引用
    M_SYNTHESIZE(int, m_iSrcUnit, SrcUnit);  // 抛射物所属单位
    M_SYNTHESIZE(int, m_iFromUnit, FromUnit);  // 发射单位
    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oFromPoint, FromPoint);
    M_SYNTHESIZE(int, m_iToUnit, ToUnit);  // 目标位点
    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oToPoint, ToPoint);

    M_SYNTHESIZE_READONLY(CAttackData*, m_pAttackData, AttackData);
    void setAttackData(CAttackData* pAttackData);
    M_SYNTHESIZE(uint32_t, m_dwTriggerMask, TriggerMask);

    M_SYNTHESIZE_READONLY(CAbility*, m_pSrcAbility, SrcAbility);
    void setSrcAbility(CAbility* pSrcAbility);

    M_SYNTHESIZE(uint32_t, m_dwEffectiveTypeFlags, EffectiveTypeFlags);

    enum PENALTY_FLAG_BIT
    {
        kOnDying = 1 << 0,
        kOnContact = 1 << 1
    };

    M_SYNTHESIZE(uint32_t, m_dwPenaltyFlags, PenaltyFlags);
    bool hasPenaltyType(PENALTY_FLAG_BIT ePenaltyType) const;

    enum FROM_TO_TYPE
    {
        kPointToUnit,
        kPointToPoint,
        kUnitToUnit,
        kUnitToPoint
    };

    M_SYNTHESIZE(FROM_TO_TYPE, m_eFromToType, FromToType);

    enum FIRE_TYPE
    {
        kFireFollow,
        kFireLink,
        kFireStraight
    };

    M_SYNTHESIZE(FIRE_TYPE, m_eFireType, FireType);

    void fireFollow(const CPoint& rFromPoint, int iToUnit, float fDuration, float fMaxHeightDelta);
    
    void fireLink(const CPoint& rFromPoint, int iToUnit);  // 点-单位
    void fireLink(const CPoint& rFromPoint, const CPoint& rToPoint);  // 点-点
    void fireLink(int iFromUnit, int iToUnit);  // 单位-单位
    void fireLink(int iFromUnit, const CPoint& rToPoint);  // 单位-点

    void fireStraight(const CPoint& rFromPoint, const CPoint& rToPoint, float fDuration, float fMaxHeightDelta);

    void fire();
    void redirect();

    typedef vector<string> VEC_SOUNDS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_SOUNDS, m_vecFireSounds, FireSounds);
    void addFireSound(const char* pSounds);
    virtual void playFireSound();

    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_SOUNDS, m_vecEffectSounds, EffectSounds);
    void addEffectSound(const char* pSounds);
    virtual void playEffectSound();

    typedef CMultiRefMap<CUnit*> MAP_CONTACTED;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_CONTACTED, m_mapContactedUnits, ContactedUnits);
    M_SYNTHESIZE(int, m_iContactLeft, ContactLeft);
    void decContactLeft(int dec = 1);
};


#endif	/* __DRAW_H__ */

