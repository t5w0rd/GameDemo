LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
../../Classes/lua/lapi.c \
../../Classes/lua/lauxlib.c \
../../Classes/lua/lbaselib.c \
../../Classes/lua/lbitlib.c \
../../Classes/lua/lcode.c \
../../Classes/lua/lcorolib.c \
../../Classes/lua/lctype.c \
../../Classes/lua/ldblib.c \
../../Classes/lua/ldebug.c \
../../Classes/lua/ldo.c \
../../Classes/lua/ldump.c \
../../Classes/lua/lfunc.c \
../../Classes/lua/lgc.c \
../../Classes/lua/linit.c \
../../Classes/lua/liolib.c \
../../Classes/lua/llex.c \
../../Classes/lua/lmathlib.c \
../../Classes/lua/lmem.c \
../../Classes/lua/loadlib.c \
../../Classes/lua/lobject.c \
../../Classes/lua/lopcodes.c \
../../Classes/lua/loslib.c \
../../Classes/lua/lparser.c \
../../Classes/lua/lstate.c \
../../Classes/lua/lstring.c \
../../Classes/lua/lstrlib.c \
../../Classes/lua/ltable.c \
../../Classes/lua/ltablib.c \
../../Classes/lua/ltm.c \
../../Classes/lua/lundump.c \
../../Classes/lua/lvm.c \
../../Classes/lua/lzio.c \
../../Classes/utils/strnormalize.c \
../../Classes/tscpdk/TSPlatform.cpp \
../../Classes/tscpdk/TSSocket.cpp \
../../Classes/tscpdk/TSUtil.cpp \
../../Classes/Logic/Ability.cpp \
../../Classes/Logic/AbilityLibrary.cpp \
../../Classes/Logic/Action.cpp \
../../Classes/Logic/Application.cpp \
../../Classes/Logic/Archive.cpp \
../../Classes/Logic/Base.cpp \
../../Classes/Logic/Draw.cpp \
../../Classes/Logic/Item.cpp \
../../Classes/Logic/Level.cpp \
../../Classes/Logic/LuaBinding.cpp \
../../Classes/Logic/LuaScriptEngine.cpp \
../../Classes/Logic/MultiRefObject.cpp \
../../Classes/Logic/Network.cpp \
../../Classes/Logic/Unit.cpp \
../../Classes/Logic/UnitLibrary.cpp \
../../Classes/AbilityForCC.cpp \
../../Classes/AbilityScene.cpp \
../../Classes/ActionForCC.cpp \
../../Classes/AppDelegate.cpp \
../../Classes/BattleScene.cpp \
../../Classes/ComponentForCC.cpp \
../../Classes/DrawForCC.cpp \
../../Classes/EmptyScene.cpp \
../../Classes/GameControl.cpp \
../../Classes/GameData.cpp \
../../Classes/HeroRoomScene.cpp \
../../Classes/LogoScene.cpp \
../../Classes/LuaBindingForCC.cpp \
../../Classes/MainMenuScene.cpp \
../../Classes/NetworkForCC.cpp \
../../Classes/Stage.cpp \
../../Classes/StageScene.cpp \
../../Classes/UnitLibraryForCC.cpp \
../../Classes/UserData.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
$(LOCAL_PATH)/../../Classes/Logic \
$(LOCAL_PATH)/../../Classes/lua \
$(LOCAL_PATH)/../../Classes/tscpdk \
$(LOCAL_PATH)/../../cocos2d

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,extensions)
