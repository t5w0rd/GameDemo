#ifndef __ACTIONFORCC_H__
#define	__ACTIONFORCC_H__


class NotifyAnimate : public Animate
{
public:
    NotifyAnimate();
    virtual ~NotifyAnimate();

    virtual bool initWithAnimation(Animation* animation, int notifyFrameIndex, const FUNC_PNODE& func, const char* sound = nullptr);
    M_CREATE_INITWITH_FUNC_PARAM(Animation, NotifyAnimate, (Animation* animation, int notifyFrameIndex, const FUNC_PNODE& func, const char* sound = nullptr), animation, notifyFrameIndex, func, sound);
    virtual void update(float t) override;

protected:
    int m_iNotifyFrameIndex;
    FUNC_PNODE m_func;
    bool m_bNotified;
    string m_sSound;
};

class FadeInOutScale4 : public Sequence
{
public:
    virtual bool init(float fScaleStart, float fScaleMid, float fScaleNormal, float fScaleEnd, float fDurToMid, float fDurToNormal, float fDurKeep, float fDurToEnd);
    M_CREATE_FUNC_PARAM(FadeInOutScale4, (float fScaleStart, float fScaleMid, float fScaleNormal, float fScaleEnd, float fDurToMid, float fDurToNormal, float fDurKeep, float fDurToEnd), fScaleStart, fScaleMid, fScaleNormal, fScaleEnd, fDurToMid, fDurToNormal, fDurKeep, fDurToEnd);

    virtual void startWithTarget(Node *pTarget) override;

protected:
    float m_fScaleStart;
};

class SequenceEx : public Sequence
{
public:
    static SequenceEx* createWithTwoActions(FiniteTimeAction *pActionOne, FiniteTimeAction *pActionTwo);
    virtual FiniteTimeAction* getActionOne();
    virtual FiniteTimeAction* getActionTwo();
};

class CUnitDrawForCC;
class CProjectileForCC;

class MoveToNode : public ActionInterval
{
public:
    enum NODE_TYPE
    {
        kNormal,
        kDraw,
        kProjectile
    };

public:
    MoveToNode();
    virtual ~MoveToNode();

    virtual bool initWithNode(float fDuration, Node* pToNode, bool bFixRotation = true, float fMaxHeightDelta = 0.0f);
    M_CREATEWITH_FUNC_PARAM(Node, MoveToNode, (float fDuration, Node* pToNode, bool bFixRotation, float fMaxHeightDelta), fDuration, pToNode, bFixRotation, fMaxHeightDelta);

    virtual bool initWithDraw(float fDuration, CUnitDrawForCC* pToDraw, bool bFixRotation = true, float fMaxHeightDelta = 0.0f);
    M_CREATEWITH_FUNC_PARAM(Draw, MoveToNode, (float fDuration, CUnitDrawForCC* pToDraw, bool bFixRotation, float fMaxHeightDelta), fDuration, pToDraw, bFixRotation, fMaxHeightDelta);

    virtual bool initWithProjectile(float fDuration, CProjectileForCC* pToProjectile, bool bFixRotation = true, float fMaxHeightDelta = 0.0f);
    M_CREATEWITH_FUNC_PARAM(Projectile, MoveToNode, (float fDuration, CProjectileForCC* pToProjectile, bool bFixRotation, float fMaxHeightDelta), fDuration, pToProjectile, bFixRotation, fMaxHeightDelta);

    virtual MoveToNode* clone() const override;

    virtual void startWithTarget(Node *pTarget) override;
    virtual void update(float time) override;
    virtual MoveToNode* reverse() const override;

protected:
    Point m_oStartPos;
    Point m_oEndPos;
    Point m_oDeltaPos;
    union
    {
        Node * m_pToNode;
        CUnitDrawForCC* m_pToDraw;
        CProjectileForCC* m_pToProjectile;
    };
    
    NODE_TYPE m_eFromType;
    NODE_TYPE m_eToType;
    float m_fMinSpeed;
    float m_fFromHeight;
    float m_fToHeight;
    float m_fDeltaHeight;
    float m_fMaxHeightDelta;
    float m_fA;
    bool m_bFixRotation;
};

class LinkAnimate : public NotifyAnimate
{
public:
    LinkAnimate();
    virtual ~LinkAnimate();
    virtual bool initWithDrawToDraw(Animation* pAnimation, int iNotifyFrameIndex, const FUNC_PNODE& func, CUnitDrawForCC* pFromDraw, CUnitDrawForCC* pToDraw);
    M_CREATEWITH_FUNC_PARAM(DrawToDraw, LinkAnimate, (Animation* pAnimation, int iNotifyFrameIndex, const FUNC_PNODE& func, CUnitDrawForCC* pFromDraw, CUnitDrawForCC* pToDraw), pAnimation, iNotifyFrameIndex, func, pFromDraw, pToDraw);
    
    virtual LinkAnimate* clone() const override;

    virtual void startWithTarget(Node *pTarget) override;
    virtual void update(float t) override;

    void fixTargetPosition(Node* pTarget);

protected:
    enum NODE_TYPE
    {
        kNormal,
        kDraw,
        kProjectile,
        kPoint
    };

    NODE_TYPE m_eFromType;
    NODE_TYPE m_eToType;
    NODE_TYPE m_eTargetType;

    union
    {
        Node* m_pFromNode;
        CUnitDrawForCC* m_pFromDraw;
        CProjectileForCC* m_pFromProjectile;
    };
    Point m_oFromPoint;

    union
    {
        Node* m_pToNode;
        CUnitDrawForCC* m_pToDraw;
        CProjectileForCC* m_pToProjectile;
    };
    Point m_oToPoint;

    bool m_bFireFrom;
};

class Shake : public Repeat
{
public:
    Shake();
    virtual bool init(float fDuration, int iTimes, float fRange);
    M_CREATE_FUNC_PARAM(Shake, (float fDuration, int iTimes, float fRange), fDuration, iTimes, fRange);

    virtual void startWithTarget(Node* pTarget);
    virtual void stop(void);


public:
    Point m_oLoc;
};

class ScaleSizeTo : public ActionInterval
{
public:
    bool initWithDuration(float duration, const Size& sz);
    M_CREATE_INITWITH_FUNC_PARAM(Duration, ScaleSizeTo, (float duration, const Size& sz), duration, sz);

    virtual ScaleSizeTo* clone() const override;
    virtual ScaleSizeTo* reverse(void) const override;
    virtual void startWithTarget(Node* target) override;
    virtual void update(float time) override;

CC_CONSTRUCTOR_ACCESS:
    ScaleSizeTo() {}
    virtual ~ScaleSizeTo() {}

protected:
    Size _scaleX;
    Size _startScaleX;
    Size _endScaleX;
    Size _deltaX;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(ScaleSizeTo);
};

class LogicJumpTo : public MoveTo
{
public:
    virtual bool initWithDuration(float duration, const Point& to, float deltaHeight);
    M_CREATE_INITWITH_FUNC_PARAM(Duration, LogicJumpTo, (float duration, const Point& to, float height), duration, to, height);

    virtual LogicJumpTo* clone() const override;

    virtual void startWithTarget(Node *pTarget) override;
    virtual void update(float time) override;
    virtual LogicJumpTo* reverse() const override;

protected:
    float m_fStartHeight;
    float m_fDeltaHeight;
    enum NODE_TYPE
    {
        kEffect,
        kDraw,
        kProjectile
    };
    NODE_TYPE m_eNodeType;
};

#endif  /* __ACTIONFORCC_H__ */
