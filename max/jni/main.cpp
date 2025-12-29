#include <jni.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <android/log.h>
#include <dlfcn.h>
#include <unistd.h>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "dobby.h"

// --- LOGGING MAKRO ---
#define LOG_TAG "MAXIM_DEBUG"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Global Vars
bool is_imgui_init = false;
typedef EGLBoolean (*eglSwapBuffers_t)(EGLDisplay dpy, EGLSurface surface);
eglSwapBuffers_t orig_eglSwapBuffers = nullptr;

// --- SETUP IMGUI ---
void SetupImGui() {
    if (is_imgui_init) return;

    LOGI("[+] Setup ImGui dimulai...");
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr; // Matikan save config
    
    ImGui::StyleColorsDark();
    ImGui::GetStyle().ScaleAllSizes(3.0f); // UI Scale Besar

    // Init OpenGL
    ImGui_ImplOpenGL3_Init("#version 100");
    LOGI("[+] ImGui Setup Selesai!");
    is_imgui_init = true;
}

// --- FUNGSI HOOK ---
EGLBoolean my_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    // Log sekali saja biar logcat tidak spam
    static bool first_frame = true;
    if (first_frame) {
        LOGI("[!!!] HOOK BERHASIL! eglSwapBuffers terpanggil!");
        first_frame = false;
    }

    SetupImGui();

    // Gambar ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::GetIO().DisplaySize = ImVec2(1080, 2400); // Ukuran dummy dulu
    ImGui::NewFrame();

    // Menu Sederhana
    ImGui::Begin("POCO X3 MOD", nullptr);
    ImGui::Text("Menu Muncul Bos!");
    if (ImGui::Button("Test Tombol")) {
        LOGI("[+] Tombol ditekan!");
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return orig_eglSwapBuffers(dpy, surface);
}

// --- FUNGSI PENCARI ALAMAT (HARDCORE MODE) ---
void *get_egl_address() {
    void *addr = nullptr;

    // CARA 1: Cari di library EGL umum
    LOGI("[?] Mencoba dlopen libEGL.so...");
    void *handle = dlopen("libEGL.so", RTLD_LAZY);
    if (handle) {
        addr = dlsym(handle, "eglSwapBuffers");
        if (addr) LOGI("[+] Ketemu di libEGL.so: %p", addr);
    }

    // CARA 2: Cari di driver ADRENO (Khusus POCO/Snapdragon)
    if (!addr) {
        LOGI("[?] Gagal. Mencoba libEGL_adreno.so...");
        handle = dlopen("libEGL_adreno.so", RTLD_LAZY);
        if (handle) {
            addr = dlsym(handle, "eglSwapBuffers");
            if (addr) LOGI("[+] KETEMU DI ADRENO: %p", addr);
        }
    }

    // CARA 3: Cari Global Symbol (RTLD_NEXT)
    if (!addr) {
        LOGI("[?] Masih gagal. Mencoba dlsym RTLD_DEFAULT...");
        addr = dlsym(RTLD_DEFAULT, "eglSwapBuffers");
        if (addr) LOGI("[+] Ketemu via Default: %p", addr);
    }

    return addr;
}

// --- CONSTRUCTOR UTAMA ---
__attribute__((constructor))
void init() {
    LOGI("==========================================");
    LOGI("   LIBMOD.SO BERHASIL DIMUAT (START)      ");
    LOGI("==========================================");

    // Cari alamat fungsi
    void *target_addr = get_egl_address();

    if (target_addr) {
        LOGI("[*] Memasang DobbyHook di: %p", target_addr);
        int result = DobbyHook(target_addr, (void *)my_eglSwapBuffers, (void **)&orig_eglSwapBuffers);
        
        if (result == 0) { // 0 biasanya sukses di Dobby/HookZz lama, atau check library spec
            LOGI("[SUCCESS] HOOK TERPASANG SEMPURNA!");
        } else {
             // Kadang Dobby tidak return status, asalkan tidak crash berarti jalan.
             LOGI("[INFO] DobbyHook dieksekusi.");
        }
    } else {
        LOGE("[FATAL] Gagal menemukan eglSwapBuffers di HP ini!");
        LOGE("[TIP] Coba cek apakah Maxim pakai Vulkan?");
    }
}