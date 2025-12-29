LOCAL_PATH := $(call my-dir)

# --- 1. SETUP DOBBY (Static) ---
include $(CLEAR_VARS)
LOCAL_MODULE := dobby_static
LOCAL_SRC_FILES := include/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)

# --- 2. SETUP MOD MENU UTAMA ---
include $(CLEAR_VARS)
LOCAL_MODULE := mod    # Hasilnya nanti libmod.so

# Flag C++ (Aktifkan fitur modern)
LOCAL_CPPFLAGS := -std=c++17 -fvisibility=hidden -fpermissive -w -DIMGUI_IMPL_OPENGL_ES2

# Include Path (Agar header ketemu)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/imgui
LOCAL_C_INCLUDES += $(LOCAL_PATH)/imgui/backends
LOCAL_C_INCLUDES += $(LOCAL_PATH)/KittyMemory

# --- SOURCE FILES (Otomatis ambil semua .cpp) ---

# A. Main Script Kita
LOCAL_SRC_FILES := main.cpp

# B. File-file ImGui (Core)
LOCAL_SRC_FILES += imgui/imgui.cpp \
                   imgui/imgui_draw.cpp \
                   imgui/imgui_tables.cpp \
                   imgui/imgui_widgets.cpp

# C. File-file ImGui (Backend Android & OpenGL3)
# Pastikan file ini ada di folder imgui/backends/
LOCAL_SRC_FILES += imgui/backends/imgui_impl_android.cpp \
                   imgui/backends/imgui_impl_opengl3.cpp

# D. File-file KittyMemory
# Ambil semua .cpp di dalam folder KittyMemory
LOCAL_SRC_FILES += $(wildcard KittyMemory/KittyMemory/*.cpp)

# --- LINKING LIBRARIES ---
# Kita butuh library grafis (EGL, GLESv2) agar ImGui bisa muncul
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2

# Gabungkan dengan Dobby
LOCAL_STATIC_LIBRARIES := dobby_static

include $(BUILD_SHARED_LIBRARY)