#include <Pentagram.hpp>

bool showDemoWindow = false;
static int step = 5;

void eventCallback(PNT::Window *window, PNT::windowEvent event) {
    static glm::vec2 movment = {0, 0};
    switch(event.type) {
        case PNT_EVENT_TYPE_KEYBOARD:
            switch(event.keyboard.key) {
            case GLFW_KEY_BACKSPACE:
                window->setPosition(700, 700);
                break;

            case GLFW_KEY_EQUAL:
                if(step < 20) step++;
                break;

            case GLFW_KEY_MINUS:
                if(step > 1) step--;
                break;

            case GLFW_KEY_RIGHT:
                if(event.keyboard.action == GLFW_PRESS) movment[0] += 1;
                if(event.keyboard.action == GLFW_RELEASE) movment[0] += -1;
                break;

            case GLFW_KEY_LEFT:
                if(event.keyboard.action == GLFW_PRESS) movment[0] += -1;
                if(event.keyboard.action == GLFW_RELEASE) movment[0] += 1;
                break;

            case GLFW_KEY_UP:
                if(event.keyboard.action == GLFW_PRESS) movment[1] += -1;
                if(event.keyboard.action == GLFW_RELEASE) movment[1] += 1;
                break;

            case GLFW_KEY_DOWN:
                if(event.keyboard.action == GLFW_PRESS) movment[1] += 1;
                if(event.keyboard.action == GLFW_RELEASE) movment[1] += -1;
                break;
            }
            window->setPosition(window->getXPos() + ((uint16_t)movment[0] * step), window->getYPos() + ((uint16_t)movment[1] * step));
            break;
        case PNT_EVENT_TYPE_CURSORPOS:
            printf("Cursor position changed to: %f, %f\n", event.cursorpos.xpos, event.cursorpos.ypos);
            break;
    }
}

int main(int argc, char *argv[]) {
    if(!PNT::init()) {
        exit(EXIT_FAILURE);
    }

    PNT::image logo("res/textures/logo/ghoul.png");
    PNT::Window window("inputTest", 500, 500, 200, 200, ImGuiConfigFlags_ViewportsEnable);
    window.setIcon(logo);
    window.setEventCallback(&eventCallback);

    while(!window.shouldClose()) {
        PNT::processEvents();

        window.startFrame();

        // Logo Path.
        ImGui::Text("Use arrow key to move window (try two directions at once)");

        // Clear Color.
        ImGui::Text("Reposition Step: ");
        ImGui::SameLine();
        ImGui::SliderInt("##SliderInt 0", &step, 1, 20, "%d", ImGuiSliderFlags_AlwaysClamp);

        // Demo Window.
        if(ImGui::Button(showDemoWindow ? "Hide Demo Window ##Button 0" : "Show Demo Window ##Button 0")) {
            showDemoWindow = !showDemoWindow;
        }
        if(showDemoWindow) {
            ImGui::ShowDemoWindow(&showDemoWindow);
        }

        window.endFrame();
    }

    PNT::deinit();
    return 0;
}
