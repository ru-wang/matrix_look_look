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
/// \file imgui_binding_opengl3.cpp
/// \brief ImGui platform binding for OpenGL.
/// \author RU WANG, rockidog@live.com
/// \date 2019-03-18

#include "./imgui_binding.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <imgui/imgui.h>

#include "./shader.hpp"
#include "./logging.hpp"

namespace g
{

static GLuint imgui_program = 0;
static GLuint imgui_font_texture = 0;
static GLuint imgui_vertex_array = 0;
static GLuint imgui_array_buffer = 0;
static GLuint imgui_element_array_buffer = 0;
static std::unordered_map<std::string, GLuint> imgui_attrib_locations;

}  /* namespace g */

void ImGui_binding_OpenGL_init()
{
    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        LOG_F(ERROR, "Error: %s", glewGetErrorString(error));
        exit(EXIT_FAILURE);
    }

    ImGui::GetIO().BackendRendererName = "ImGui binding for OpenGL";

    g::imgui_program = glCreateProgram();

    /* shaders */
    {
        shader vs(ImGui_vertex_shader_text, GL_VERTEX_SHADER);
        shader fs(ImGui_fragment_shader_text, GL_FRAGMENT_SHADER);
        vs.attach(g::imgui_program);
        fs.attach(g::imgui_program);
        glLinkProgram(g::imgui_program);
        vs.detach(g::imgui_program);
        fs.detach(g::imgui_program);
    }

    /* attribute locations */
    {
        g::imgui_attrib_locations["Texture"]  = glGetUniformLocation(g::imgui_program, "Texture");
        g::imgui_attrib_locations["ProjMtx"]  = glGetUniformLocation(g::imgui_program, "ProjMtx");
        g::imgui_attrib_locations["Position"] = glGetAttribLocation(g::imgui_program, "Position");
        g::imgui_attrib_locations["UV"]       = glGetAttribLocation(g::imgui_program, "UV");
        g::imgui_attrib_locations["Color"]    = glGetAttribLocation(g::imgui_program, "Color");
    }

    /* font texture */
    {
        unsigned char* font_pixels;
        int width, height;
        ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&font_pixels, &width, &height);

        glGenTextures(1, &g::imgui_font_texture);
        glBindTexture(GL_TEXTURE_2D, g::imgui_font_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, font_pixels);

        ImGui::GetIO().Fonts->TexID = (ImTextureID)(intptr_t)g::imgui_font_texture;
    }

    /* VAO VBO EBO */
    {
        glGenVertexArrays(1, &g::imgui_vertex_array);
        glGenBuffers(1, &g::imgui_array_buffer);
        glGenBuffers(1, &g::imgui_element_array_buffer);

        glBindVertexArray(g::imgui_vertex_array);
        glBindBuffer(GL_ARRAY_BUFFER, g::imgui_array_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g::imgui_element_array_buffer);

        glVertexAttribPointer(
                g::imgui_attrib_locations["Position"],
                2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert),
                (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
        glVertexAttribPointer(
                g::imgui_attrib_locations["UV"],
                2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert),
                (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
        glVertexAttribPointer(
                g::imgui_attrib_locations["Color"],
                4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert),
                (GLvoid*)IM_OFFSETOF(ImDrawVert, col));

        glEnableVertexAttribArray(g::imgui_attrib_locations["Position"]);
        glEnableVertexAttribArray(g::imgui_attrib_locations["UV"]);
        glEnableVertexAttribArray(g::imgui_attrib_locations["Color"]);
    }
}

void ImGui_binding_OpenGL_destroy()
{
    if (g::imgui_font_texture)
        glDeleteTextures(1, &g::imgui_font_texture);
    if (g::imgui_vertex_array)
        glDeleteVertexArrays(1, &g::imgui_vertex_array);
    if (g::imgui_element_array_buffer)
        glDeleteBuffers(1, &g::imgui_element_array_buffer);
    if (g::imgui_element_array_buffer)
        glDeleteBuffers(1, &g::imgui_element_array_buffer);
    if (g::imgui_program)
        glDeleteProgram(g::imgui_program);
    ImGui::GetIO().Fonts->TexID = 0;
}

void ImGui_binding_OpenGL_render(ImDrawData* draw_data)
{
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
        return;

    glViewport(0, 0, fb_width, fb_height);

    GLfloat L = draw_data->DisplayPos.x;
    GLfloat R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    GLfloat T = draw_data->DisplayPos.y;
    GLfloat B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
    const GLfloat ortho[4][4] =
    {
        { 2.0f/(R-L),   0.0f,         0.0f,   0.0f },
        { 0.0f,         2.0f/(T-B),   0.0f,   0.0f },
        { 0.0f,         0.0f,        -1.0f,   0.0f },
        { (R+L)/(L-R),  (T+B)/(B-T),  0.0f,   1.0f },
    };

    glUseProgram(g::imgui_program);
    glUniform1i(g::imgui_attrib_locations["Texture"], 0);
    glUniformMatrix4fv(g::imgui_attrib_locations["ProjMtx"], 1, GL_FALSE, &ortho[0][0]);

    ImVec2 clip_off = draw_data->DisplayPos;
    ImVec2 clip_scale = draw_data->FramebufferScale;

    GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
    GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
    GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
    GLboolean last_enable_blend = glIsEnabled(GL_BLEND);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);

    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(g::imgui_vertex_array);
    for (int i = 0; i < draw_data->CmdListsCount; ++i)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[i];
        size_t idx_buffer_offset = 0;

        glBindBuffer(GL_ARRAY_BUFFER, g::imgui_array_buffer);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size*sizeof(ImDrawVert),
                (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g::imgui_element_array_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size*sizeof(ImDrawIdx),
                (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; ++cmd_i)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                ImVec4 clip_rect;
                clip_rect.x = (pcmd->ClipRect.x-clip_off.x)*clip_scale.x;
                clip_rect.y = (pcmd->ClipRect.y-clip_off.y)*clip_scale.y;
                clip_rect.z = (pcmd->ClipRect.z-clip_off.x)*clip_scale.x;
                clip_rect.w = (pcmd->ClipRect.w-clip_off.y)*clip_scale.y;
                if (clip_rect.x < fb_width && clip_rect.y < fb_height &&
                    clip_rect.z >= 0.0 && clip_rect.w >= 0.0)
                {
                    GLint x = clip_rect.x, y = fb_height - clip_rect.w;
                    GLsizei width  = clip_rect.z - clip_rect.x,
                            height = clip_rect.w - clip_rect.y;
                    glScissor(x, y, width, height);
                    glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                    if (sizeof(ImDrawIdx) == 2)
                    {
                        glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
                                GL_UNSIGNED_SHORT, (void*)idx_buffer_offset);
                    }
                    else
                    {
                        glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
                                GL_UNSIGNED_INT, (void*)idx_buffer_offset);
                    }
                }
            }
            idx_buffer_offset += pcmd->ElemCount * sizeof(ImDrawIdx);
        }
    }

    last_enable_depth_test   ? glEnable(GL_DEPTH_TEST)   : glDisable(GL_DEPTH_TEST);
    last_enable_cull_face    ? glEnable(GL_CULL_FACE)    : glDisable(GL_CULL_FACE);
    last_enable_blend        ? glEnable(GL_BLEND)        : glDisable(GL_BLEND);
    last_enable_scissor_test ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);
}
