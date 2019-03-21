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
/// \file imgui_magic.hpp
/// \brief ImGui magic.
/// \author RU WANG, rockidog@live.com
/// \date 2019-03-18

#pragma once

struct ImVec4;

enum ImGui_info_flags : int
{
    ImGui_info_flags_NONE                        = 0,
    ImGui_info_flags_SIZE                        = 1 << 0,
    ImGui_info_flags_MIN                         = 1 << 1,
    ImGui_info_flags_MAX                         = 1 << 2,
    ImGui_info_flags_SUM                         = 1 << 3,
    ImGui_info_flags_MIN_ABS                     = 1 << 4,
    ImGui_info_flags_MAX_ABS                     = 1 << 5,
    ImGui_info_flags_L1_NORM                     = 1 << 6,
    ImGui_info_flags_L2_NORM                     = 1 << 7,
    ImGui_info_flags_SHOW_EIGENVALUES            = 1 << 8,
    ImGui_info_flags_REORDER_EIGENVALUES         = 1 << 9,
    ImGui_info_flags_REORDER_EIGENVALUES_ABS     = 1 << 10,
    ImGui_info_flags_SHOW_SINGULAR_VALUES        = 1 << 11,
    ImGui_info_flags_SHOW_CHOLESKY_PIVOTS        = 1 << 12,
    ImGui_info_flags_REORDER_CHOLESKY_PIVOTS     = 1 << 13,
    ImGui_info_flags_REORDER_CHOLESKY_PIVOTS_ABS = 1 << 14,
    ImGui_info_flags_DETERMINANT                 = 1 << 15,
    ImGui_info_flags_CONDITION_NUMBER            = 1 << 16,
};

void ImGui_render_matrix_look_look(const double* data, const int rows, const int cols, const float rect_size, const int info_flags);
