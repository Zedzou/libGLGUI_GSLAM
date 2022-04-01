#include "CoordinateAxis.h"

using namespace GLRenderer;

CoordinateAxis::~CoordinateAxis()
{
    // 初始化完进行删除
    if(bInited) 
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
}

void CoordinateAxis::Init()
{
    std::string vsShader = "version 330\n"
                          "layout(location=0) in vec3 position;\n"
                          "uniform mat4 model;\n"
                          "uniform mat4 view;\n"
                          "uniform mat4 projection;\n"
                          "void main():{\n"
                          "    gl_Position = projection * view * model * vec4(position, 1.0);\n"
                          "}\n";
    
    std::string fsShader = "version 330\n"
                          "out vec4 FragColor;\n"
                          "uniform vec4 color;\n"
                          "void main():{\n"
                          "    FragColor=color;\n"
                          "}\n";
    
    const float size = 0.5f;
    float vertices[] = {0.0f,      0.0f,      0.0f,
                        1.0f*size, 0.0f,      0.0f,
                        0.0f,      1.0f*size, 0.0f,
                        0.0f,      0.0f,      1.0f*size};

    unsigned int indices[] = {0, 1, 0, 2, 0, 3};

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glBindVertexArray(0);

    // Init shader
    mShader = std::make_unique<glUtil::Shader>(vsShader,fsShader);
    mShader->use();
    mShader->set("color", color);
    mShader->set("model", glm::mat4(1.f));
    bInited=true; // 控制析构函数删除缓存
}

void CoordinateAxis::Draw(Eigen::Matrix4f model, const Eigen::Matrix4f &projection, const Eigen::Matrix4f &viewMatrix)
{
    assert(bInited);
    model = model * mScale;// glm::scale(model,glm::vec3(scale, scale, scale));    // it's a bit too big for our scene, so scale it down

    mShader->use();
    mShader->set("color", color);
    mShader->set("model",model);
    mShader->set("view",viewMatrix);
    mShader->set("projection",projection);

    glBindVertexArray(VAO);
    glLineWidth(3);
    glDrawElements(GL_LINES, 16, GL_UNSIGNED_INT, 0);
    glLineWidth(1);
}