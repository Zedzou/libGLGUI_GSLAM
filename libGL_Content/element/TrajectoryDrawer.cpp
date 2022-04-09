#include "TrajectoryDrawer.h"
using namespace GLRenderer;

void TrajectoryDrawer::Init()
{

    const std::string vsShader = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "uniform mat4 model;\n"
                                 "uniform mat4 view;\n"
                                 "uniform mat4 projection;\n"
                                 "void main()\n"
                                 "{\n"
                                     "gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
                                 "}\n";

    const std::string fsShader = "#version 330 core\n"
                                 "out vec4 FragColor;\n"
                                 "uniform vec4 color;\n"
                                 "void main()\n"
                                 "{\n"
                                     "FragColor = color;\n"   
                                 "}\n\0";

    mShader = std::make_unique<GLUtils::Shader>(vsShader, fsShader);
    mShader->use();
    mShader->set("color", Eigen::Vector4f{0, 1, 0, 1.f});
    mShader->set("model", glm::mat4(1.f));
}
