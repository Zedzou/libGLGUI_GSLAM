#include "libGL_UI/GL_UI.h"
#include "libGL_Content/GL_Content.h"

int main()
{
    std::string name = "test";
    GLRenderer::GL_Content gui_gslam(name, 800, 600);
    gui_gslam.run();
    return 0;
}