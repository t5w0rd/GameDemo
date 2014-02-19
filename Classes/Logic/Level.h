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
    virtual void updateMaxExp(CLevelExp* pLevel) = 0; // @override
    virtual void onLevelChange(CLevelExp* pLevel, int iChanged) = 0; // @override
};

// 等级经验值，赋予对象等级经验值特性
// 需要覆盖 updateMaxExp，提供等级变化时的最大经验值变更公式
// 等级变化后触发 onLevelChange
class CLevelExp
{
public:
    CLevelExp();
    virtual ~CLevelExp();
    
    virtual void updateMaxExp(); // @override
    virtual void onChangeLevel(int iChanged); // @override
    void addExp(int iExp);
    void addLevel(int iLvl);
    virtual void setLevel(int iLvl);
    virtual void setMaxLevel(int iMaxLvl);
    
    int getLevel() const;
    int getMaxLevel() const;
    int getExp() const;
    int getMaxExp() const;
    
    void setLevelUpdate(CLevelUpdate* pUpdate);
    
public:
    int m_iLvl;
    int m_iMaxLvl;
    int m_iExp;
    int m_iMaxExp;
    CLevelUpdate* m_pUpdate;
    
};


#endif	/* __LEVEL_H__ */

