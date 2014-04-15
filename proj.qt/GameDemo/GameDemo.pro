#-------------------------------------------------
#
# Project created by QtCreator 2014-04-15T16:29:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GameDemo
TEMPLATE = app


SOURCES += main.cpp \
    ../../Classes/UserData.cpp \
    ../../Classes/UnitLibraryForCC.cpp \
    ../../Classes/TestScene.cpp \
    ../../Classes/MainMenuScene.cpp \
    ../../Classes/LuaBindingForCC.cpp \
    ../../Classes/LogoScene.cpp \
    ../../Classes/HeroRoomScene.cpp \
    ../../Classes/GameControl.cpp \
    ../../Classes/DrawForCC.cpp \
    ../../Classes/ComponentForCC.cpp \
    ../../Classes/BattleScene.cpp \
    ../../Classes/AppDelegate.cpp \
    ../../Classes/ActionForCC.cpp \
    ../../Classes/AbilityForCC.cpp \
    ../../Classes/Logic/Unit.cpp \
    ../../Classes/Logic/MultiRefObject.cpp \
    ../../Classes/Logic/LuaBinding.cpp \
    ../../Classes/Logic/Level.cpp \
    ../../Classes/Logic/Item.cpp \
    ../../Classes/Logic/Draw.cpp \
    ../../Classes/Logic/BingAbility.cpp \
    ../../Classes/Logic/Base.cpp \
    ../../Classes/Logic/Application.cpp \
    ../../Classes/Logic/Action.cpp \
    ../../Classes/Logic/Ability.cpp \
    ../../Classes/lua/lzio.c \
    ../../Classes/lua/lvm.c \
    ../../Classes/lua/lundump.c \
    ../../Classes/lua/ltm.c \
    ../../Classes/lua/ltablib.c \
    ../../Classes/lua/ltable.c \
    ../../Classes/lua/lstrlib.c \
    ../../Classes/lua/lstring.c \
    ../../Classes/lua/lstate.c \
    ../../Classes/lua/lparser.c \
    ../../Classes/lua/loslib.c \
    ../../Classes/lua/lopcodes.c \
    ../../Classes/lua/lobject.c \
    ../../Classes/lua/loadlib.c \
    ../../Classes/lua/lmem.c \
    ../../Classes/lua/lmathlib.c \
    ../../Classes/lua/llex.c \
    ../../Classes/lua/liolib.c \
    ../../Classes/lua/linit.c \
    ../../Classes/lua/lgc.c \
    ../../Classes/lua/lfunc.c \
    ../../Classes/lua/ldump.c \
    ../../Classes/lua/ldo.c \
    ../../Classes/lua/ldebug.c \
    ../../Classes/lua/ldblib.c \
    ../../Classes/lua/lctype.c \
    ../../Classes/lua/lcorolib.c \
    ../../Classes/lua/lcode.c \
    ../../Classes/lua/lbitlib.c \
    ../../Classes/lua/lbaselib.c \
    ../../Classes/lua/lauxlib.c \
    ../../Classes/lua/lapi.c

HEADERS += \
    ../../Classes/UserData.h \
    ../../Classes/UnitLibraryForCC.h \
    ../../Classes/TestScene.h \
    ../../Classes/MainMenuScene.h \
    ../../Classes/LuaBindingForCC.h \
    ../../Classes/LogoScene.h \
    ../../Classes/HeroRoomScene.h \
    ../../Classes/GameControl.h \
    ../../Classes/DrawForCC.h \
    ../../Classes/ComponentForCC.h \
    ../../Classes/CommHeader.h \
    ../../Classes/BattleScene.h \
    ../../Classes/AppDelegate.h \
    ../../Classes/ActionForCC.h \
    ../../Classes/AbilityForCC.h \
    ../../Classes/Logic/Unit.h \
    ../../Classes/Logic/MultiRefObject.h \
    ../../Classes/Logic/LuaBinding.h \
    ../../Classes/Logic/Level.h \
    ../../Classes/Logic/Item.h \
    ../../Classes/Logic/Draw.h \
    ../../Classes/Logic/CommInc.h \
    ../../Classes/Logic/CommDef.h \
    ../../Classes/Logic/BingAbility.h \
    ../../Classes/Logic/Base.h \
    ../../Classes/Logic/Application.h \
    ../../Classes/Logic/Action.h \
    ../../Classes/Logic/Ability.h \
    ../../Classes/lua/lzio.h \
    ../../Classes/lua/lvm.h \
    ../../Classes/lua/lundump.h \
    ../../Classes/lua/lualib.h \
    ../../Classes/lua/luaconf.h \
    ../../Classes/lua/lua.h \
    ../../Classes/lua/ltm.h \
    ../../Classes/lua/ltable.h \
    ../../Classes/lua/lstring.h \
    ../../Classes/lua/lstate.h \
    ../../Classes/lua/lparser.h \
    ../../Classes/lua/lopcodes.h \
    ../../Classes/lua/lobject.h \
    ../../Classes/lua/lmem.h \
    ../../Classes/lua/llimits.h \
    ../../Classes/lua/llex.h \
    ../../Classes/lua/lgc.h \
    ../../Classes/lua/lfunc.h \
    ../../Classes/lua/ldo.h \
    ../../Classes/lua/ldebug.h \
    ../../Classes/lua/lctype.h \
    ../../Classes/lua/lcode.h \
    ../../Classes/lua/lauxlib.h \
    ../../Classes/lua/lapi.h \
    ../../Classes/lua/lua.hpp

INCLUDEPATH += $$PWD/../../../../cocos2dx \
    $$PWD/../../../../cocos2dx/include \
    $$PWD/../../../../cocos2dx/kazmath/include \
    $$PWD/../../../../cocos2dx/platform/win32 \
    $$PWD/../../../../cocos2dx/platform/third_party/win32 \
    $$PWD/../../../../cocos2dx/platform/third_party/win32/iconv \
    $$PWD/../../../../cocos2dx/platform/third_party/win32/OGLES \
    $$PWD/../../../../CocosDenshion/Include \
    $$PWD/../../../../external \
    $$PWD/../../../../external/chipmunk/include \
    $$PWD/../../../../extensions \
    $$PWD/../../Classes \
    $$PWD/../../Classes/Logic \
    $$PWD/../../Classes/lua

COCOS2DX_LIBS = -llibcocos2d -llibBox2d -llibCocosDenshion -llibExtensions -lopengl32 -lglew32 -llibiconv

win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../proj.win32/Debug.win32/ $$COCOS2DX_LIBS
else:win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../proj.win32/Release.win32/ $$COCOS2DX_LIBS
else:unix: LIBS += -L$$PWD/../../../proj.win32/Debug.win32/ $$COCOS2DX_LIBS

win32:CONFIG(debug, debug|release): DEFINES += QTPROJ WIN32 _DEBUG _WINDOWS COCOS2D_DEBUG=1 _CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS DEBUG_FOR_CC
else:win32:CONFIG(release, debug|release): DEFINES += QTPROJ WIN32 NDEBUG _WINDOWS _CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS DEBUG_FOR_CC

CONFIG += warn_off

OTHER_FILES += \
    ../../Resources/scripts/world.lua \
    ../../Resources/scripts/common.lua \
    ../../Resources/scripts/ai2.lua \
    ../../Resources/scripts/ai.lua \
    ../../Resources/scripts/ability.lua
