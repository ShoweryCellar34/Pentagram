#include <Pentagram.hpp>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#include <shobjidl.h>

// Ripped for microsoft.
PWSTR userGetPath() {
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr)) {
        IFileOpenDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr)) {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr)) {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr)) {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr)) {
                        return pszFilePath;
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    return PWSTR();
}
#endif

PNT::image image;

void eventCallback(PNT::Window* window, PNT::windowEvent event) {
    switch(event.type) {
    case PNT_EVENT_TYPE_DROP:
        image.load(event.dropFiles.paths[0]);
        window->setDimentions(image.getWidth(), image.getHeight());
        window->setAspectRatio(image.getWidth(), image.getHeight());
        image.loadOnGPU();
        break;
    case PNT_EVENT_TYPE_KEYBOARD:
        if(event.keyboard.action == GLFW_PRESS) {
            switch(event.keyboard.key) {
            case GLFW_KEY_R:
                window->setDimentions(image.getWidth(), image.getHeight());
                break;
            case GLFW_KEY_ESCAPE:
                window->setShouldClose(true);
                break;
#ifdef _WIN32
            case GLFW_KEY_TAB:
                PWSTR wpath = userGetPath();
                if(wpath != nullptr) {
                    size_t origsize = wcslen(wpath) + 1;
                    size_t convertedChars = 0;
                    const size_t newsize = origsize * 2;
                    char* path = new char[newsize];
                    wcstombs_s(&convertedChars, path, newsize, wpath, _TRUNCATE);

                    image.load(path);
                    window->setDimentions(image.getWidth(), image.getHeight());
                    window->setAspectRatio(image.getWidth(), image.getHeight());
                    image.loadOnGPU();

                    delete[] path;
                }
                CoTaskMemFree(wpath);
                break;
#endif
            }
        }
    }
}

int main(int argc, char *argv[]) {
    stbi_set_flip_vertically_on_load(true);

    if(!PNT::init()) {
        return 1;
    }

    image.load("res/textures/logo/ghoul.png");
    PNT::Window window("Drag'n Drop", image.getWidth(), image.getHeight(), 500, 500, 0);
    image.loadOnGPU();
    window.setAspectRatio(image.getWidth(), image.getHeight());
    window.setEventCallback(eventCallback);

    // Vertex shader.
    PNT::file file("res/shaders/vertex.glsl");
    printf("%s\n", file.getError());
    PNT::shader vertexShader(file.getContents().c_str(), GL_VERTEX_SHADER);
    vertexShader.compile();
    std::cout << vertexShader.getError() << '\n';

    // Fragment shader.
    file.close();
    file.open("res/shaders/fragment.glsl");
    printf("%s\n", file.getError());
    PNT::shader fragmentShader(file.getContents().c_str(), GL_FRAGMENT_SHADER);
    fragmentShader.compile();
    std::cout << fragmentShader.getError() << '\n';

    // Shader program.
    PNT::program shader({&vertexShader, &fragmentShader});
    shader.link();
    std::cout << shader.getError() << '\n';

    float vertices[] = {
         1.0f,  1.0f,
         1.0f, -1.0f,
        -1.0f, -1.0f,
        -1.0f,  1.0f
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUniform1i(glGetUniformLocation(shader.getID(), "texture"), image.getID());

    while(!window.shouldClose()) {
        PNT::processEvents();

        window.startFrame();

        if (ImGui::BeginMainMenuBar()) {
              if (ImGui::BeginMenu("File")) {
#ifdef _WIN32
                    if (ImGui::MenuItem("Open", "TAB")) {
                        PWSTR wpath = userGetPath();
                        if(wpath != nullptr) {
                            size_t origsize = wcslen(wpath) + 1;
                            size_t convertedChars = 0;
                            const size_t newsize = origsize * 2;
                            char* path = new char[newsize];
                            wcstombs_s(&convertedChars, path, newsize, wpath, _TRUNCATE);

                            image.load(path);
                            window.setDimentions(image.getWidth(), image.getHeight());
                            window.setAspectRatio(image.getWidth(), image.getHeight());
                            image.loadOnGPU();

                            delete[] path;
                        }
                        CoTaskMemFree(wpath);
                    }
#endif
#ifndef _WIN32
                        ImGui::Text("File open dialog only avalable on windows");
#endif
        ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
        }

        shader.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.endFrame();
    }

    //PNT::deinit();
    return 0;
}
