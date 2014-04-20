#ifndef __STAGE_SCENE_H__
#define __STAGE_SCENE_H__


#include "Stage.h"

class CCStageSceneLayer : public CCLayerColor
{
public:
    CCStageSceneLayer();
    virtual ~CCStageSceneLayer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(CCStageSceneLayer);

    CStageMap m_oStageMap;

    void onClickStage(CCObject* pObj);
    void onClickBattle(CCObject* pObj);

    CCLabelTTF* m_pName;
    CCLabelTTF* m_pDesc;

    int m_iSelIndex;
    CCButtonNormal* m_pBattle;
};


#endif // __STAGE_SCENE_H__
