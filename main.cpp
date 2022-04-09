#include "libGL_ContentBase/GL_ContentBase.h"

int main()
{
    std::string name = "test";
    GLRenderer::GL_ContentBase win(name, 800, 600);
    win.run();
    return 0;
}
