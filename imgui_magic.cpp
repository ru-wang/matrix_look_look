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
/// \file imgui_magic.cpp
/// \brief ImGui magic.
/// \author RU WANG, rockidog@live.com
/// \date 2019-03-18

#include "./imgui_magic.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <utility>
#include <vector>

#include <Eigen/Eigen>
#include <imgui/imgui.h>

namespace g
{

static ImVec4 zero_color(0.0f, 0.5f, 0.5f, 0.5f);
static ImVec4 min_color(0.0f, 0.0f, 0.0f, 0.5f);
static ImVec4 max_color(1.0f, 1.0f, 1.0f, 0.5f);

static double time = 0.0;

}  /* namespace g */

struct run_once_if_static
{
    run_once_if_static(const std::function<void()>& f)
    { f(); }
};

void add_vertical_space(float w)
{
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(w, 0.0f));
}

void render_vsliders_for_color(ImVec4& color, const char* label)
{
    {
        ImGui::PushID(label);
        ImGui::PushStyleColor(ImGuiCol_FrameBg,          ImVec4(1.0f, 0.0f, 0.0f, 0.2f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,   ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive,    ImVec4(1.0f, 0.0f, 0.0f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrab,       ImVec4(1.0f, 0.0f, 0.0f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        ImGui::VSliderFloat("##r", ImVec2(20.0f, 180.0f), &color.x, 0.0f, 1.0f, "");
        if (ImGui::IsItemActive() || ImGui::IsItemHovered())
            ImGui::SetTooltip("R: %.1f", color.x);
        ImGui::PopStyleColor(5);
        ImGui::PopID();
    }
    ImGui::SameLine();
    {
        ImGui::PushID(label);
        ImGui::PushStyleColor(ImGuiCol_FrameBg,          ImColor(0.0f, 1.0f, 0.0f, 0.2f).Value);
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,   ImColor(0.0f, 1.0f, 0.0f, 0.4f).Value);
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive,    ImColor(0.0f, 1.0f, 0.0f, 0.6f).Value);
        ImGui::PushStyleColor(ImGuiCol_SliderGrab,       ImColor(0.0f, 1.0f, 0.0f, 0.8f).Value);
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImColor(0.0f, 1.0f, 0.0f, 1.0f).Value);
        ImGui::VSliderFloat("##g", ImVec2(20.0f, 180.0f), &color.y, 0.0f, 1.0f, "");
        if (ImGui::IsItemActive() || ImGui::IsItemHovered())
            ImGui::SetTooltip("G: %.1f", color.y);
        ImGui::PopStyleColor(5);
        ImGui::PopID();
    }
    ImGui::SameLine();
    {
        ImGui::PushID(label);
        ImGui::PushStyleColor(ImGuiCol_FrameBg,          ImColor(0.0f, 0.0f, 1.0f, 0.2f).Value);
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,   ImColor(0.0f, 0.0f, 1.0f, 0.4f).Value);
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive,    ImColor(0.0f, 0.0f, 1.0f, 0.6f).Value);
        ImGui::PushStyleColor(ImGuiCol_SliderGrab,       ImColor(0.0f, 0.0f, 1.0f, 0.8f).Value);
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImColor(0.0f, 0.0f, 1.0f, 1.0f).Value);
        ImGui::VSliderFloat("##b", ImVec2(20.0f, 180.0f), &color.z, 0.0f, 1.0f, "");
        if (ImGui::IsItemActive() || ImGui::IsItemHovered())
            ImGui::SetTooltip("B: %.1f", color.z);
        ImGui::PopStyleColor(5);
        ImGui::PopID();
    }
}

namespace two_columns
{

void begin(const char* lhs, const char* rhs)
{
    ImGui::Columns(2, nullptr, true);
    ImGui::Separator();
    ImGui::TextDisabled("%s", lhs);
    ImGui::NextColumn();
    ImGui::TextDisabled("%s", rhs);
}

template<typename... args_type>
void middle_line(bool has, const char* name, const char* fmt, args_type&&... args)
{
    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Text("%s", name);
    ImGui::NextColumn();
    if (has)
        ImGui::Text(fmt, std::forward<args_type>(args)...);
    else
        ImGui::Text("N/A");
}

template<typename... args_type>
void middle_line_lhs(const char* fmt, args_type&&... args)
{
    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Text(fmt, std::forward<args_type>(args)...);
}

template<typename... args_type>
void middle_line_rhs(const char* fmt, args_type&&... args)
{
    ImGui::NextColumn();
    ImGui::Text(fmt, std::forward<args_type>(args)...);
}

void end()
{
    ImGui::NextColumn();
    ImGui::Columns();
    ImGui::Separator();
}

} /* namespace two_columns */

void ImGui_render_matrix_look_look(const double* data, const int rows, const int cols, const float rect_size, const int info_flags)
{
    double fps = 1.0 / (ImGui::GetTime() - g::time);
    g::time = ImGui::GetTime();

    Eigen::Map<Eigen::MatrixXd const> mapped(data, rows, cols);
    Eigen::MatrixXd dmat = (mapped.array() == 0.0).select(0.0, mapped.array().abs());
    double max = dmat.maxCoeff();
    double min = max;
    for (int i = 0; i < dmat.size(); ++i)
        if (dmat.data()[i] != 0.0 && dmat.data()[i] < min)
            min = dmat.data()[i];
    dmat = (dmat.array() == 0.0).select(-std::numeric_limits<double>::infinity(), dmat.array().log10());
    min = std::log10(min);
    max = std::log10(max) - min;

    std::vector<double> colored_matrix(rows*cols, -1.0);
    for (int i = 0, k = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j, ++k)
            if (std::isfinite(dmat(i, j)))
                colored_matrix.at(k) = (dmat(i, j) - min) / max;

    const ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSizeConstraints(io.DisplaySize, io.DisplaySize);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoResize;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0.0f);
    if (not ImGui::Begin("MatrixLookLook", nullptr, window_flags))
    {
        ImGui::End();
        ImGui::PopStyleVar(2);
        return;
    }

    ImGui::Columns(2, nullptr, true);

    static run_once_if_static a_static_snippet([](){ ImGui::SetColumnWidth(0, 350.0f); });

    ImGui::BeginChild("info panel", ImVec2(0.0f, 0.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
    {
        ImGui::Text("FPS: %.2f", fps);

        ImGui::Separator();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0, 4.0f));

        render_vsliders_for_color(g::zero_color, "ZeroColor");
        add_vertical_space(10.0f);
        ImGui::SameLine();
        render_vsliders_for_color(g::min_color, "MinColor");
        add_vertical_space(10.0f);
        ImGui::SameLine();
        render_vsliders_for_color(g::max_color, "MaxColor");

        ImVec2 button_size(68.0f, 68.0f);
        ImGuiColorEditFlags color_flags = ImGuiColorEditFlags_NoPicker;
        ImGui::ColorButton("color for zero value", g::zero_color, color_flags, button_size);
        add_vertical_space(10.0f);
        ImGui::SameLine();
        ImGui::ColorButton("color for min value", g::min_color, color_flags, button_size);
        add_vertical_space(10.0f);
        ImGui::SameLine();
        ImGui::ColorButton("color for max value", g::max_color, color_flags, button_size);

        ImGui::PopStyleVar();

        two_columns::begin("name", "value");
        static run_once_if_static a_static_snippet([](){ ImGui::SetColumnWidth(0, 100.0f); });
        two_columns::middle_line(info_flags & ImGui_info_flags_SIZE,    "size",    "%dx%d", rows, cols);
        two_columns::middle_line(info_flags & ImGui_info_flags_MIN,     "min",     "%f",    mapped.minCoeff());
        two_columns::middle_line(info_flags & ImGui_info_flags_MAX,     "max",     "%f",    mapped.maxCoeff());
        two_columns::middle_line(info_flags & ImGui_info_flags_SUM,     "sum",     "%f",    mapped.sum());
        two_columns::middle_line(info_flags & ImGui_info_flags_MIN_ABS, "min abs", "%f",    mapped.cwiseAbs().minCoeff());
        two_columns::middle_line(info_flags & ImGui_info_flags_MAX_ABS, "max abs", "%f",    mapped.cwiseAbs().maxCoeff());
        two_columns::middle_line(info_flags & ImGui_info_flags_L1_NORM, "L1-norm", "%f",    mapped.lpNorm<1>());
        two_columns::middle_line(info_flags & ImGui_info_flags_L2_NORM, "L2-norm", "%f",    mapped.lpNorm<2>());
        two_columns::end();

        if (info_flags & ImGui_info_flags_SHOW_EIGENVALUES)
        {
            if (ImGui::TreeNode("eigenvalues"))
            {
                auto eigenvalues = Eigen::EigenSolver<Eigen::MatrixXd>(mapped, false).eigenvalues();
                std::vector<int> indices(eigenvalues.size());
                for (int i = 0; i < (int)indices.size(); ++i)
                    indices.at(i) = i;
                if (info_flags & ImGui_info_flags_REORDER_EIGENVALUES)
                    std::sort(indices.begin(), indices.end(), [&eigenvalues](int i, int j)
                            { return (eigenvalues[i].real() > eigenvalues[j].real()); });
                else if (info_flags & ImGui_info_flags_REORDER_EIGENVALUES_ABS)
                    std::sort(indices.begin(), indices.end(), [&eigenvalues](int i, int j)
                            { return (std::abs(eigenvalues[i].real()) > std::abs(eigenvalues[j].real())); });

                two_columns::begin("pivot", "eigenvalue");
                static run_once_if_static a_static_snippet([](){ ImGui::SetColumnWidth(0, 60.0f); });
                for (auto i : indices)
                {
                    two_columns::middle_line_lhs("[%d]", i);
                    two_columns::middle_line_rhs("%f", eigenvalues[i]);
                }
                two_columns::end();

                ImGui::TreePop();
            }
        }

        if (info_flags & ImGui_info_flags_SHOW_SINGULAR_VALUES)
        {
            if (ImGui::TreeNode("singular values"))
            {
                auto singular_values = Eigen::JacobiSVD<Eigen::MatrixXd>(mapped).singularValues();
                std::vector<int> indices(singular_values.size());
                for (int i = 0; i < (int)indices.size(); ++i)
                    indices.at(i) = i;

                two_columns::begin("pivot", "singular value");
                static run_once_if_static a_static_snippet([](){ ImGui::SetColumnWidth(0, 60.0f); });
                for (auto i : indices)
                {
                    two_columns::middle_line_lhs("[%d]", i);
                    two_columns::middle_line_rhs("%f", singular_values[i]);
                }
                two_columns::end();

                ImGui::TreePop();
            }
        }
    }
    ImGui::EndChild();

    ImGui::NextColumn();

    ImGui::BeginChild("matrix view", ImVec2(0.0f, 0.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 anchor = ImGui::GetCursorScreenPos();
        ImGui::InvisibleButton("MatrixRect", ImVec2(rows*rect_size, cols*rect_size));
        Eigen::Vector4d color_start(g::min_color.x, g::min_color.y, g::min_color.z, g::min_color.w);
        Eigen::Vector4d color_range(g::max_color.x-g::min_color.x, g::max_color.y-g::min_color.y,
                g::max_color.z-g::min_color.z, g::max_color.w-g::min_color.w);
        for (int i = 0, k = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j, ++k)
            {
                ImVec2 p0(anchor.x+j*rect_size, anchor.y+i*rect_size);
                ImVec2 p1(anchor.x+(j+1)*rect_size-1.0, anchor.y+(i+1)*rect_size-1.0);
                if (colored_matrix.at(k) < 0.0)
                {
                    draw_list->AddRectFilled(p0, p1, ImGui::ColorConvertFloat4ToU32(g::zero_color));
                }
                else
                {
                    Eigen::Vector4d c = color_start + color_range * colored_matrix.at(k);
                    draw_list->AddRectFilled(p0, p1, ImGui::ColorConvertFloat4ToU32(ImVec4(c[0], c[1], c[2], c[3])));
                }
                if (ImGui::IsMouseHoveringRect(p0, ImVec2(p1.x+1.0, p1.y+1.0)))
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("[%d,%d] = %e\n", i, j, mapped(i, j));
                    ImGui::EndTooltip();
                }
            }
        }
    }
    ImGui::EndChild();

    ImGui::Columns(1);

    ImGui::End();
    ImGui::PopStyleVar(2);
}
