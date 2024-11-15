// main.cpp
#define WIN32_LEAN_AND_MEAN
#define IMGUI_DISABLE_DEBUG_TOOLS

#include <windows.h>
#include <gl/gl.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"

bool darkTheme = true;
bool showOptions = false;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) {
        return true;
    }

    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

bool InitializeOpenGL(HWND hwnd) {
    HDC hdc = GetDC(hwnd);
    if (!hdc) {
        return false;
    }

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cRedBits = 8;
    pfd.cGreenBits = 8;
    pfd.cBlueBits = 8;
    pfd.cAlphaBits = 8;

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    if (!pixelFormat || !SetPixelFormat(hdc, pixelFormat, &pfd)) {
        return false;
    }

    HGLRC hglrc = wglCreateContext(hdc);
    if (!hglrc || !wglMakeCurrent(hdc, hglrc)) {
        return false;
    }

    return true;
}

void Render(HWND hwnd) {
    HDC hdc = GetDC(hwnd);

    glClear(GL_COLOR_BUFFER_BIT);
    if (darkTheme == true) {
        ImGui::StyleColorsDark();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    } else {
        ImGui::StyleColorsLight();
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // topbar
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::EndMenu(); // placeholder
        }
        if (ImGui::BeginMenu("Edit")) {
            ImGui::EndMenu(); // placeholder
        }
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About")) {

            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));

    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()));
    ImGui::SetNextWindowSize(ImVec2(200, ImGui::GetIO().DisplaySize.y - ImGui::GetFrameHeight()));
    ImGui::Begin("Leftbar", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Spacing();

    ImGui::PopStyleColor();

    float button_width = ImGui::CalcTextSize("Options").x + ImGui::GetStyle().FramePadding.x * 2;
    ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - button_width - ImGui::GetStyle().FramePadding.x);
    ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - ImGui::GetStyle().FramePadding.y - ImGui::GetFrameHeightWithSpacing());

    if (ImGui::Button("Options")) {
        showOptions = !showOptions;
    }

    ImGui::End();

    if (showOptions) {
        ImGui::SetNextWindowPos(ImVec2((ImGui::GetIO().DisplaySize.x - 300) * 0.5f, (ImGui::GetIO().DisplaySize.y - 200) * 0.5f), ImGuiCond_Once);
        ImGui::OpenPopup("Options");
    }

    if (ImGui::BeginPopup("Options")) {
        ImGui::Text("Options");

        if (ImGui::Button("Light Theme"))
            darkTheme = false;

        ImGui::SameLine();

        if (ImGui::Button("Dark Theme"))
            darkTheme = true;

        if (ImGui::Button("Close")) {
            showOptions = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 590, ImGui::GetFrameHeight()));
    ImGui::SetNextWindowSize(ImVec2(600, ImGui::GetIO().DisplaySize.y - ImGui::GetFrameHeight())); 
    ImGui::Begin("Rightbar", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Spacing();

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SwapBuffers(hdc);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "Void Modding Tool";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Void Modding Tool",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr, nullptr, hInstance, nullptr
    );

    if (hwnd == nullptr) {
        return 0;
    }

    if (!InitializeOpenGL(hwnd)) {
        return 0;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::GetIO().IniFilename = nullptr;

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplOpenGL3_Init("#version 130");

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        Render(hwnd);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    return 0;
}