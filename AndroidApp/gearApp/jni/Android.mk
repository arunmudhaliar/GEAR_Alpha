LOCAL_PATH := $(call my-dir)

#include $(CLEAR_VARS)
#LOCAL_MODULE    := monomodule
#LOCAL_SRC_FILES := ../../gearengineAndroid/monoandroid-arm/lib/libmonosgen-2.0.a
#LOCAL_EXPORT_C_INCLUDES := ../gearengineAndroid/monoandroid-arm/include/mono-2.0
#include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := gearmodule
LOCAL_EXPORT_C_INCLUDES := ../gearengineAndroid/monoandroid-arm/include/mono-2.0
LOCAL_SRC_FILES := ../../gearengineAndroid/libs/armeabi/libGEAREngine.dll.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := gearapp
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2 -lOpenSLES
LOCAL_STATIC_LIBRARIES := android_native_app_glue
LOCAL_SHARED_LIBRARIES := gearmodule
LOCAL_SRC_FILES := gearapp.cpp
APP_OPTIM := debug
include $(BUILD_SHARED_LIBRARY)
$(call import-module, android/native_app_glue)