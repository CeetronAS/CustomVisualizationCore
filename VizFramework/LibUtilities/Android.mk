
LOCAL_PATH:= $(call my-dir)

# Build CeeViz LibUtilities
# -------------------------------------
include $(CLEAR_VARS)
LOCAL_MODULE := CeeVizUtilities
LOCAL_CFLAGS := -DCVF_ANDROID -DCVF_OPENGL_ES -Werror
FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
FILE_LIST := $(subst "$(LOCAL_PATH)/", "\n", $(FILE_LIST))
FILE_LIST := $(filter-out %PartCompoundGenerator.cpp, $(FILE_LIST))
LOCAL_SRC_FILES := $(FILE_LIST)
LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../LibCore \
	$(LOCAL_PATH)/../LibGeometry \
	$(LOCAL_PATH)/../LibIo \
	$(LOCAL_PATH)/../LibRender \
	$(LOCAL_PATH)/../LibViewing
LOCAL_SHARED_LIBRARIES := CeeVizCore CeeVizGeometry CeeVizIo CeeVizRender CeeVizViewing
include $(BUILD_SHARED_LIBRARY)
