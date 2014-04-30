/* 
 * File:   Level.h
 * Author: thunderliu
 *
 * Created on 2013年12月8日, 下午10:31
 */

#ifndef __LEVEL_H__
#define	__LEVEL_H__


class CLevelExp;

class CLevelUpdate
{
public:
    virtual void updateExpRange(CLevelExp* pLevel) = 0; // @override
    virtual void onChangeLevel(CLevelExp* pLevel, int iChanged) = 0; // @override
};

// 等级经验值，赋予对象等级经验值特性
// 需要覆盖 updateExpRange，提供等级变化时的最大经验值变更公式
// 等级变化后触发 onChangeLevel
class CLevelExp
{
public:
    CLevelExp();
    virtual ~CLevelExp();
    
    virtual void updateExpRange(); // @override
    virtual void onChangeLevel(int iChanged); // @override
    void addExp(int iExp);
    void addLevel(int iLvl);
    
    M_SYNTHESIZE_READONLY(int, m_iLvl, Level);
    M_SYNTHESIZE_READONLY(int, m_iMaxLvl, MaxLevel);
    M_SYNTHESIZE(int, m_iExp, Exp);
    M_SYNTHESIZE(int, m_iBaseExp, BaseExp);
    M_SYNTHESIZE(int, m_iMaxExp, MaxExp);
    M_SYNTHESIZE_READONLY(CLevelUpdate*, m_pUpdate, Update);
    
    void setLevel(int iLvl);
    void setMaxLevel(int iMaxLvl);
    void setLevelUpdate(CLevelUpdate* pUpdate);
    bool canIncreaseExp() const;
};


#endif	/* __LEVEL_H__ */

