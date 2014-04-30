#ifndef __STAGE_SCENE_H__
#define __STAGE_SCENE_H__


#include "Stage.h"

class CCStageSceneLayer : public CCWinLayer
{
public:
    CCStageSceneLayer();
    virtual ~CCStageSceneLayer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void onLoadingProgress(CCObject* pObj);
    void onLoadingDone(CCObject* pObj);

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(CCStageSceneLayer);

    CStageMap m_stageMap;

    void onClickStage(CCObject* pObj);
    void onClickPanelBattle(CCObject* pObj);
    void onClickPanelClose(CCObject* pObj);

    CCTouchMaskLayer* m_ctrlLayer;
    CCPopPanel* m_panel;
    
    CCLabelTTF* m_name;
    CCLabelTTF* m_desc;
    CCSprite* m_stars[3];

    int m_selIndex;
    CCButtonNormal* m_btnBattle;

    CCSprite* m_stageCur;
};


#endif // __STAGE_SCENE_H__
