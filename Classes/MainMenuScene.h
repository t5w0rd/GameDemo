#ifndef __MainMenu_SCENE_H__
#define __MainMenu_SCENE_H__

class MainMenuScene : public Scene
{
public:
    MainMenuScene();
    virtual ~MainMenuScene();

    virtual bool init();
    CREATE_FUNC(MainMenuScene);

};

class SelectArchiveLayer;

class MainMenuSceneLayer : public Layer
{
public:
    MainMenuSceneLayer();
    virtual ~MainMenuSceneLayer();

    virtual bool init();
    virtual void onEnter();
    static cocos2d::Scene* scene();
    CREATE_FUNC(MainMenuSceneLayer);

    void MenuItemStartCallback(Ref* sender);
    void MenuItemLogoCallback(Ref* sender);
    void CCCallSelectArchiveLayer();
    void CCRunStartAction();
private:
    MenuItemImage *m_pItemStart;
};

class SelectArchiveLayer : public Layer
{
public:
    SelectArchiveLayer();
    virtual ~SelectArchiveLayer();

    virtual bool init();
    virtual void onEnter();
    CREATE_FUNC(SelectArchiveLayer);

    void MenuItemCallback1(Ref* sender);
    void MenuItemCallback2(Ref* sender);
    void MenuItemCallback3(Ref* sender);
    void MenuItemCloseCallback(Ref* sender);
    void MenuItemDeleteCallback(Ref* sender);

private:
    MenuItemImage* m_pItem1;
    MenuItemImage* m_pItem2;
    MenuItemImage* m_pItem3;
    Label* m_pLabel1;
    Label* m_pLabel2;
    Label* m_pLabel3;
};
/*
class SaveFile : public Ref
{
public:
    SaveFile();
    virtual ~SaveFile();

    CREATE_FUNC(SaveFile);
    virtual bool init(const char* pFileName, const char* pMode);
    M_CREATE_FUNC_PARAM(SaveFile, (const char* pFileName, const char* pMode), pFileName, pMode);
    void readSaveFile();
    void updateSaveFile(const char* name, const char* value);
    //void addSaveData(const char* name, const char* value);
    //int getValue(const char* name, string& value);
//protected:
    map<string,string> m_DataMap;
};
*/

#endif