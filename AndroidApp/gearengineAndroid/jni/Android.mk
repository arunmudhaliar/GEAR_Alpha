LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := monomodule
LOCAL_SRC_FILES := ../monoandroid-arm/lib/libmonosgen-2.0.a
LOCAL_EXPORT_C_INCLUDES := monoandroid-arm/include/mono-2.0
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := GEAREngine.dll
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/../../../GEAREngine/src/physics $(LOCAL_PATH)/../../../GEAREngine/src
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2 -lOpenSLES
LOCAL_STATIC_LIBRARIES := monomodule
LOCAL_SRC_FILES := \
	../../../GEAREngine/src/GEAREngine.cpp	\
	../../../GEAREngine/src/audio/SoundEngine.cpp	\
	../../../GEAREngine/src/audio/SoundSample.cpp	\
	../../../GEAREngine/src/audio/SoundSource.cpp	\
	../../../GEAREngine/src/core/Camera.cpp	\
	../../../GEAREngine/src/core/COctree.cpp	\
	../../../GEAREngine/src/core/fbo.cpp	\
	../../../GEAREngine/src/core/GEARAsset.cpp	\
	../../../GEAREngine/src/core/gxAnimation.cpp	\
	../../../GEAREngine/src/core/gxAnimationSet.cpp	\
	../../../GEAREngine/src/core/gxAnimationTrack.cpp	\
	../../../GEAREngine/src/core/gxCamera.cpp	\
	../../../GEAREngine/src/core/gxDebug.cpp	\
	../../../GEAREngine/src/core/gxLight.cpp	\
	../../../GEAREngine/src/core/gxMaterial.cpp	\
	../../../GEAREngine/src/core/gxMesh.cpp	\
	../../../GEAREngine/src/core/gxSkinnedMesh.cpp	\
	../../../GEAREngine/src/core/gxSurfaceShader.cpp	\
	../../../GEAREngine/src/core/gxWorld.cpp	\
	../../../GEAREngine/src/core/LayerManager.cpp	\
	../../../GEAREngine/src/core/object3d.cpp	\
	../../../GEAREngine/src/core/physicsEngine.cpp	\
	../../../GEAREngine/src/core/TextureManager.cpp	\
	../../../GEAREngine/src/core/Timer.cpp	\
	../../../GEAREngine/src/core/transform.cpp	\
	../../../GEAREngine/src/hwShader/gxHWShader.cpp	\
	../../../GEAREngine/src/hwShader/HWShaderManager.cpp	\
	../../../GEAREngine/src/mono/src/monoScript.cpp	\
	../../../GEAREngine/src/mono/src/monoWrapper.cpp	\
	../../../GEAREngine/src/renderer/gxRenderer.cpp	\
	../../../GEAREngine/src/util/FontManager.cpp	\
	../../../GEAREngine/src/util/gxCrc32.cpp	\
	../../../GEAREngine/src/util/gxFile.cpp	\
	../../../GEAREngine/src/util/gxUtil.cpp	\
	../../../GEAREngine/src/zlib-1.2.4/adler32.c	\
	../../../GEAREngine/src/zlib-1.2.4/compress.c	\
	../../../GEAREngine/src/zlib-1.2.4/crc32.c	\
	../../../GEAREngine/src/zlib-1.2.4/deflate.c	\
	../../../GEAREngine/src/zlib-1.2.4/infback.c	\
	../../../GEAREngine/src/zlib-1.2.4/inffast.c	\
	../../../GEAREngine/src/zlib-1.2.4/inflate.c	\
	../../../GEAREngine/src/zlib-1.2.4/inftrees.c	\
	../../../GEAREngine/src/zlib-1.2.4/trees.c	\
	../../../GEAREngine/src/zlib-1.2.4/uncompr.c	\
	../../../GEAREngine/src/zlib-1.2.4/zutil.c
include $(BUILD_SHARED_LIBRARY)
