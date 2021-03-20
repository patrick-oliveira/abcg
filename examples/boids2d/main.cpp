#include <fmt/core.h>

#include "abcg.hpp"
#include "openglwindow.hpp" // Classe openglwindow, definida em outro arquivo
                            // Note que é possível definir uma classe em um arquivo .hpp para ser importada por outros arquivos

int main(int argc, char **argv) {
    try{
        abcg::Application app(argc, argv);

        auto window{std::make_unique<OpenGLWindow>()};
        window -> setOpenGLSettings({.samples = 2, 
                                     .vsync = true,
                                     .preserveWebGLDrawingBuffer = true});

        window -> setWindowSettings({.width = 1600, 
                                     .height = 800, 
                                     .showFPS = false,
                                     .showFullscreenButton = false,
                                     .title = "Boids"});

        app.run(window);
    } catch (abcg::Exception &exception){
        fmt::print(stderr, "{}\n", exception.what());
        return -1;
    }
    return 0;
}