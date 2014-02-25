LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := gamedemo/main.cpp \
../../Classes/AbilityForCC.cpp \
../../Classes/ActionForCC.cpp \
../../Classes/AppDelegate.cpp \
../../Classes/BattleScene.cpp \
../../Classes/ComponentForCC.cpp \
../../Classes/DrawForCC.cpp \
../../Classes/GameControl.cpp \
../../Classes/Logic/Ability.cpp \
../../Classes/Logic/Action.cpp \
../../Classes/Logic/Application.cpp \
../../Classes/Logic/Base.cpp \
../../Classes/Logic/Draw.cpp \
../../Classes/Logic/Item.cpp \
../../Classes/Logic/Level.cpp \
../../Classes/Logic/MultiRefObject.cpp \
../../Classes/Logic/Unit.cpp


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
