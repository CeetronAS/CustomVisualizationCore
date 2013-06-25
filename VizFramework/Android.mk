LOCAL_PATH:= $(call my-dir)

# Howto build
# -------------------------------------
# cd <to jni folder>
# C:\Android\android-ndk-r8\ndk-build clean
# C:\Android\android-ndk-r8\ndk-build

CEEVIZ_ROOT_PATH := $(LOCAL_PATH)
include $(CEEVIZ_ROOT_PATH)/ThirdParty/ZipArchive/Android.mk
include $(CEEVIZ_ROOT_PATH)/LibCore/Android.mk
include $(CEEVIZ_ROOT_PATH)/LibGeometry/Android.mk
include $(CEEVIZ_ROOT_PATH)/LibIo/Android.mk
include $(CEEVIZ_ROOT_PATH)/LibRender/Android.mk
include $(CEEVIZ_ROOT_PATH)/LibViewing/Android.mk
include $(CEEVIZ_ROOT_PATH)/LibUtilities/Android.mk
include $(CEEVIZ_ROOT_PATH)/LibUnstructGrid/Android.mk
include $(CEEVIZ_ROOT_PATH)/LibUnstructDisplay/Android.mk
include $(CEEVIZ_ROOT_PATH)/LibUnstructVTFx/Android.mk

