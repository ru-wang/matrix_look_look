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
/// \file shader.cpp
/// \brief GLSL shaders.
/// \author RU WANG, rockidog@live.com
/// \date 2019-03-17

#include "./shader.hpp"

const GLchar* ImGui_vertex_shader_text =
R"(
    #version 330 core
    #extension GL_ARB_separate_shader_objects : enable
    layout (location = 0) in vec2 Position;
    layout (location = 1) in vec2 UV;
    layout (location = 2) in vec4 Color;
    uniform mat4 ProjMtx;
    out vec2 Frag_UV;
    out vec4 Frag_Color;
    void main()
    {
        Frag_UV = UV;
        Frag_Color = Color;
        gl_Position = ProjMtx * vec4(Position.xy,0,1);
    }
)";

const GLchar* ImGui_fragment_shader_text =
R"(
    #version 330 core
    #extension GL_ARB_separate_shader_objects : enable
    in vec2 Frag_UV;
    in vec4 Frag_Color;
    uniform sampler2D Texture;
    layout (location = 0) out vec4 Out_Color;
    void main()
    {
        Out_Color = Frag_Color * texture(Texture, Frag_UV.st);
    };
)";
