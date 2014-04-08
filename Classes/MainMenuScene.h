#ifndef __MainMenu_SCENE_H__
#define __MainMenu_SCENE_H__

class CCMainMenuScene : public CCScene
{
public:
    CCMainMenuScene();
    virtual ~CCMainMenuScene();

    virtual bool init();
    CREATE_FUNC(CCMainMenuScene);

};

class CCSelectArchiveLayer;

class CCMainMenuSceneLayer : public CCLayer
{
public:
    CCMainMenuSceneLayer();
    virtual ~CCMainMenuSceneLayer();

    virtual bool init();
    virtual void onEnter();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(CCMainMenuSceneLayer);

    void CCMenuItemStartCallback(CCObject* sender);
    void CCMenuItemLogoCallback(CCObject* sender);
    void CCCallSelectArchiveLayer();
    void CCRunStartAction();
private:
    CCMenuItemImage *m_pItemStart;
};

class CCSelectArchiveLayer : public CCLayer
{
public:
    CCSelectArchiveLayer();
    virtual ~CCSelectArchiveLayer();

    virtual bool init();
    virtual void onEnter();
    CREATE_FUNC(CCSelectArchiveLayer);

    void CCMenuItemCallback1(CCObject* sender);
    void CCMenuItemCallback2(CCObject* sender);
    void CCMenuItemCallback3(CCObject* sender);
    void CCMenuItemCloseCallback(CCObject* sender);
    void CCMenuItemDeleteCallback(CCObject* sender);

private:
    CCMenuItemImage* m_pItem1;
    CCMenuItemImage* m_pItem2;
    CCMenuItemImage* m_pItem3;
    CCLabelTTF* m_pLabel1;
    CCLabelTTF* m_pLabel2;
    CCLabelTTF* m_pLabel3;
};

class CCSaveFile : public CCObject
{
public:
    CCSaveFile();
    virtual ~CCSaveFile();

    CREATE_FUNC(CCSaveFile);
    virtual bool init(const char* pFileName, const char* pMode);
    M_CREATE_FUNC_PARAM(CCSaveFile, (const char* pFileName, const char* pMode), pFileName, pMode);
    void readSaveFile();
    void updateSaveFile(const char* name, const char* value);
    //void addSaveData(const char* name, const char* value);
    //int getValue(const char* name, string& value);
//protected:
    map<string,string> m_DataMap;
};


#endif