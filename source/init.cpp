#include <init.hpp>

namespace PNT {

    bool init() {
        return glfwInit();
    }
    void deinit() {
        glfwTerminate();
    }
}
