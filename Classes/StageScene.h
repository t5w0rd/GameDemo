#ifndef __STAGE_SCENE_H__
#define __STAGE_SCENE_H__

#include "Stage.h"
#include "ComponentForCC.h"


class StageSceneLayer : public WinLayer
{
public:
    StageSceneLayer();
    virtual ~StageSceneLayer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void onLoadingProgress();
    void onLoadingDone();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(StageSceneLayer);

    CStageMap m_stageMap;

    void onClickStage(Ref* pObj);
    void onClickPanelBattle(Ref* pObj);
    void onClickPanelClose(Ref* pObj);

    TouchMaskLayer* m_ctrlLayer;
    PopPanel* m_panel;
    
    Label* m_name;
    Label* m_desc;
    Sprite* m_stars[3];

    int m_selIndex;
    ButtonNormal* m_btnBattle;

    Sprite* m_stageCur;
};


#endif // __STAGE_SCENE_H__
