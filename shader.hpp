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
/// \file shader.hpp
/// \brief GLSL shaders.
/// \author RU WANG, rockidog@live.com
/// \date 2019-03-17

#pragma once

#include <cstdlib>
#include <GL/glew.h>
#include "./logging.hpp"

extern const GLchar* ImGui_vertex_shader_text;
extern const GLchar* ImGui_fragment_shader_text;

struct shader
{
    shader(const char shader_text[], GLenum shader_type)
    {
        shader_name = glCreateShader(shader_type);
        glShaderSource(shader_name, 1, &shader_text, nullptr);
        glCompileShader(shader_name);
        GLint compile_status;
        glGetShaderiv(shader_name, GL_COMPILE_STATUS, &compile_status);

        if (compile_status == GL_FALSE)
        {
            int info_log_len;
            glGetShaderiv(shader_name, GL_INFO_LOG_LENGTH, &info_log_len);
            GLchar* info_log = new GLchar[info_log_len+1];
            glGetShaderInfoLog(shader_name, info_log_len, nullptr, info_log);
            info_log[info_log_len] = '\0';
            LOG_F(ERROR, "%s", shader_text);
            LOG_F(ERROR, "%s", info_log);
            delete [] info_log;
            exit(EXIT_FAILURE);
        }
    }

    ~shader()
    {
        glDeleteShader(shader_name);
    }

    void attach(GLuint program)
    {
        glAttachShader(program, shader_name);
    }

    void detach(GLuint program)
    {
        glDetachShader(program, shader_name);
    }

    GLuint shader_name = 0;
};
