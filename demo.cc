#include <Eigen/Eigen>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include "./imgui_binding.hpp"
#include "./imgui_magic.hpp"
#include "./logging.hpp"

int main() {
    if (not glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwSetErrorCallback([](int error, const char* description)
            { LOG_F(ERROR, "Error %d: %s", error, description); });
    GLFWwindow* window = glfwCreateWindow(
            1280, 720, "MATRIX LOOK LOOK", nullptr, nullptr);
    if (not window)
    {
        LOG_F(ERROR, "Failed to create window");
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui::GetIO().Fonts->AddFontFromFileTTF("Roboto-Regular.ttf", 18);
    ImVec4 clear_color(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui_binding_GLFW_init(window);
    ImGui_binding_OpenGL_init();

    Eigen::MatrixXd m = Eigen::MatrixXd::Random(40, 40);
    m.diagonal().setZero();

    while (not glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_binding_GLFW_new_frame(window);
        ImGui::NewFrame();

        int info_flags = ImGui_info_flags_SIZE |
                         ImGui_info_flags_MIN |
                         ImGui_info_flags_MAX |
                         ImGui_info_flags_SUM |
                         ImGui_info_flags_MIN_ABS |
                         ImGui_info_flags_MAX_ABS |
                         ImGui_info_flags_L1_NORM |
                         ImGui_info_flags_L2_NORM |
                         ImGui_info_flags_SHOW_EIGENVALUES |
                         ImGui_info_flags_REORDER_EIGENVALUES_ABS |
                         ImGui_info_flags_SHOW_SINGULAR_VALUES;

        ImGui_render_matrix_look_look(m.data(), m.rows(), m.cols(), 20.0f, info_flags);

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);

        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_binding_OpenGL_render(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_binding_OpenGL_destroy();
    ImGui_binding_GLFW_destroy(window);

    glfwTerminate();

    return 0;
}
