#include "libGL_Content/GL_Content.h"

int main()
{
    std::string name = "test";
    GLRenderer::GL_Content win(name, 800, 600);
    win.run();
    return 0;
}
