#include "CommHeader.h"
#include "GameData.h"
#include "GameControl.h"


// CGameData
CGameData::CGameData()
{
    STAGE_INFO stage;

    // stage 0
    stage.name = "The Sunwell";
    stage.desc = "The Sunwell is a fount of mystical power located in Quel\'Thalas";
    stage.btnNorName = "UI/Stage/Stage00Nor.png";
    stage.btnDisName = "UI/Stage/Stage00Dis.png";
    stage.pos = Point(200, 200);
    stage.prevIndex.clear();
    stage.script = "stage00.lua";
    stage.background = "backgrounds/BackgroundHD01.png";
    m_stages.push_back(stage);

    // stage 1
    stage.name = "The Death of the Captain";
    stage.desc = "It was created from a vial of water from the Well of Eternity";
    stage.btnNorName = "UI/Stage/Stage01Nor.png";
    stage.btnDisName = "UI/Stage/Stage01Dis.png";
    stage.pos = Point(500, 200);
    stage.prevIndex.clear();
    stage.prevIndex.push_back(0);
    stage.script = "stage01.lua";
    stage.background = "backgrounds/BackgroundHD01.png";
    m_stages.push_back(stage);

    // stage 2
    stage.name = "Strike Back";
    stage.desc = "It empowered the high elves for approximately nine thousand years, until Arthas used it to resurrect Kel\'Thuzad as a lich";
    stage.btnNorName = "UI/Stage/Stage02Nor.png";
    stage.btnDisName = "UI/Stage/Stage02Dis.png";
    stage.pos = Point(800, 200);
    stage.prevIndex.clear();
    stage.prevIndex.push_back(1);
    stage.script = "stage02.lua";
    stage.background = "backgrounds/BackgroundHD01.png";
    m_stages.push_back(stage);

    // stage 3
    stage.name = "Doubt";
    stage.desc = "His resurrection tainted the Sunwell in the process, severing the high elves from their source of power";
    stage.btnNorName = "UI/Stage/Stage03Nor.png";
    stage.btnDisName = "UI/Stage/Stage03Dis.png";
    stage.pos = Point(500, 550);
    stage.prevIndex.clear();
    stage.prevIndex.push_back(1);
    stage.script = "stage03.lua";
    stage.background = "backgrounds/BackgroundHD01.png";
    m_stages.push_back(stage);

    // stage 4
    stage.name = "Siege";
    stage.desc = "Kael\'thas attempted to summon Kil\'jaeden into Azeroth through the Sunwell using Anveena\'s powers";
    stage.btnNorName = "UI/Stage/Stage04Nor.png";
    stage.btnDisName = "UI/Stage/Stage04Dis.png";
    stage.pos = Point(1100, 200);
    stage.prevIndex.clear();
    stage.prevIndex.push_back(2);
    stage.prevIndex.push_back(3);
    stage.script = "stage04.lua";
    stage.background = "backgrounds/BackgroundHD01.png";
    m_stages.push_back(stage);

    // stage 5
    stage.name = "The End";
    stage.desc = "In World of Warcraft, the Sunwell appears in the level 70 raid dungeon, Sunwell Plateau, on the Isle of Quel\'Danas";
    stage.btnNorName = "UI/Stage/Stage05Nor.png";
    stage.btnDisName = "UI/Stage/Stage05Dis.png";
    stage.pos = Point(1100, 550);
    stage.prevIndex.clear();
    stage.prevIndex.push_back(4);
    stage.script = "stage05.lua";
    stage.background = "backgrounds/BackgroundHD01.png";
    m_stages.push_back(stage);
}
