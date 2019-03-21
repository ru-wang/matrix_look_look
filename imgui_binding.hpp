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
/// \file imgui_binding.hpp
/// \brief ImGui platform bindings for OpenGL and GLFW.
/// \author RU WANG, rockidog@live.com
/// \date 2019-03-18

#pragma once

struct GLFWwindow;
struct ImDrawData;

void ImGui_binding_GLFW_init(GLFWwindow* window);
void ImGui_binding_GLFW_destroy(GLFWwindow* window);
void ImGui_binding_GLFW_new_frame(GLFWwindow* window);

void ImGui_binding_GLFW_update_mouse(GLFWwindow* window);
void ImGui_binding_GLFW_update_cursors(GLFWwindow* window);

void ImGui_binding_OpenGL_init();
void ImGui_binding_OpenGL_destroy();
void ImGui_binding_OpenGL_new_frame();
void ImGui_binding_OpenGL_render(ImDrawData* draw_data);
