/// This file is a part of juzhenkankan.
/// Copyright (C) 2018  RU WANG
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
///
/// \file imgui_binding_glfw.cpp
/// \brief ImGui platform binding for GLFW.
/// \author RU WANG, rockidog@live.com
/// \date 2019-03-16

#include "./imgui_binding.hpp"

#include <array>
#include <cstdint>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include "./logging.hpp"

namespace g
{

static std::array<unsigned char, 5> mouse_just_pressed = { 0 };
static std::array<GLFWcursor*, ImGuiMouseCursor_COUNT> mouse_cursors = { 0 };
static double time = 0.0;

} /* namespace g */

const char* GLFW_get_clipboard_text(void* user_data)
{
    return glfwGetClipboardString((GLFWwindow*)user_data);
}

void GLFW_set_clipboard_text(void* user_data, const char* text)
{
    glfwSetClipboardString((GLFWwindow*)user_data, text);
}

void GLFW_mouse_button_callback(GLFWwindow* /*window*/, int button, int action, int /*mods*/)
{
    if (action == GLFW_PRESS &&
        button >= 0 &&
        button < (int)g::mouse_just_pressed.size())
    {
        g::mouse_just_pressed[button] = true;
    }
}

void GLFW_mouse_scroll_callback(GLFWwindow* /*window*/, double xoffset, double yoffset)
{
    ImGui::GetIO().MouseWheelH += xoffset;
    ImGui::GetIO().MouseWheel  += yoffset;
}

void GLFW_key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
    ImGuiIO& io = ImGui::GetIO();
    if (action == GLFW_PRESS)
        io.KeysDown[key] = true;
    else if (action == GLFW_RELEASE)
        io.KeysDown[key] = false;
    io.KeyCtrl  = (io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL]);
    io.KeyShift = (io.KeysDown[GLFW_KEY_LEFT_SHIFT]   || io.KeysDown[GLFW_KEY_RIGHT_SHIFT]);
    io.KeyAlt   = (io.KeysDown[GLFW_KEY_LEFT_ALT]     || io.KeysDown[GLFW_KEY_RIGHT_ALT]);
    io.KeySuper = (io.KeysDown[GLFW_KEY_LEFT_SUPER]   || io.KeysDown[GLFW_KEY_RIGHT_SUPER]);

    if (io.KeyCtrl && io.KeysDown[GLFW_KEY_Q])
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void GLFW_char_callback(GLFWwindow* /*window*/, unsigned int ch)
{
    if (ch > 0 && ch < 0x10000)
        ImGui::GetIO().AddInputCharacter(ch);
}

void ImGui_binding_GLFW_init(GLFWwindow* window)
{
    g::time = glfwGetTime();

    ImGuiIO& io = ImGui::GetIO();
    io.BackendPlatformName = "ImGui binding for GLFW";
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

    io.KeyMap[ImGuiKey_Tab]        = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow]  = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow]    = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow]  = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp]     = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown]   = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home]       = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End]        = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Insert]     = GLFW_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete]     = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace]  = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space]      = GLFW_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter]      = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape]     = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A]          = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C]          = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V]          = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X]          = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y]          = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z]          = GLFW_KEY_Z;

    io.SetClipboardTextFn = GLFW_set_clipboard_text;
    io.GetClipboardTextFn = GLFW_get_clipboard_text;
    io.ClipboardUserData  = window;

    glfwSetMouseButtonCallback(window, GLFW_mouse_button_callback);
    glfwSetScrollCallback(window, GLFW_mouse_scroll_callback);
    glfwSetKeyCallback(window, GLFW_key_callback);
    glfwSetCharCallback(window, GLFW_char_callback);

    g::mouse_cursors[ImGuiMouseCursor_Arrow]      = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    g::mouse_cursors[ImGuiMouseCursor_TextInput]  = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    g::mouse_cursors[ImGuiMouseCursor_ResizeAll]  = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    g::mouse_cursors[ImGuiMouseCursor_ResizeNS]   = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
    g::mouse_cursors[ImGuiMouseCursor_ResizeEW]   = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    g::mouse_cursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    g::mouse_cursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    g::mouse_cursors[ImGuiMouseCursor_Hand]       = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
}

void ImGui_binding_GLFW_destroy(GLFWwindow* /*window*/)
{
    for (ImGuiMouseCursor i = 0; i < ImGuiMouseCursor_COUNT; ++i)
    {
        glfwDestroyCursor(g::mouse_cursors[i]);
        g::mouse_cursors[i] = nullptr;
    }
}

void ImGui_binding_GLFW_new_frame(GLFWwindow* window)
{
    ImGuiIO& io = ImGui::GetIO();
    if (not io.Fonts->IsBuilt())
        LOG_F(ERROR, "Font atlas not built");

    int w, h;
    int display_w, display_h;
    glfwGetWindowSize(window, &w, &h);
    glfwGetFramebufferSize(window, &display_w, &display_h);
    io.DisplaySize = ImVec2(w, h);
    if (w > 0 && h > 0)
        io.DisplayFramebufferScale = ImVec2(display_w*1.0/w, display_h*1.0/h);

    double current_time = glfwGetTime();
    io.DeltaTime = (g::time > 0.0) ? (current_time-g::time) : (1.0/60.0);
    g::time = current_time;

    ImGui_binding_GLFW_update_mouse(window);
    ImGui_binding_GLFW_update_cursors(window);
}

void ImGui_binding_GLFW_update_mouse(GLFWwindow* window)
{
    ImGuiIO& io = ImGui::GetIO();

    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); ++i)
    {
        io.MouseDown[i] = g::mouse_just_pressed[i] || glfwGetMouseButton(window, i);
        g::mouse_just_pressed[i] = 0;
    }

    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
    if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
    {
        double mouse_x, mouse_y;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        io.MousePos = ImVec2(mouse_x, mouse_y);
    }
}

void ImGui_binding_GLFW_update_cursors(GLFWwindow* window)
{
    ImGuiIO& io = ImGui::GetIO();

    if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) ||
        glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
    {
        return;
    }

    ImGuiMouseCursor cursor = ImGui::GetMouseCursor();
    if (cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
    else
    {
        if (g::mouse_cursors[cursor])
            glfwSetCursor(window, g::mouse_cursors[cursor]);
        else
            glfwSetCursor(window, g::mouse_cursors[ImGuiMouseCursor_Arrow]);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
