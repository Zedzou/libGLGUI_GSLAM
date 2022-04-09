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
    const char* vsShader = "#version 330 core\n"
                           "layout(location=0) in vec3 position;\n"
                           "layout(location=1) in vec3 acolor;\n"
                           "out vec3 fColor;\n"
                           "uniform mat4 model;\n"      // uniform设置model：渲染的物体旋转平移转换矩阵
                           "uniform mat4 view;\n"       // uniform设置view: 视角矩阵
                           "uniform mat4 projection;\n" // uniform设置projection:投影矩阵
                           "void main()\n"
                           "{\n"
                           "    gl_Position = projection * view * model * vec4(position, 1.0);\n"
                           "    fColor = acolor;\n"
                           "}\0";
    
    const char* fsShader = "#version 330 core\n"
                           "in vec3 fColor;\n"
                           "out vec4 FragColor;\n"
                           "void main()\n"
                           "{\n"
                           "    FragColor=vec4(fColor, 1.0f);\n"
                           "}\n\0";
    
    const float size = 0.5f;
    float vertices[] = {0.0f,      0.0f,      0.0f,      1.0f, 0.0f, 0.0f,
                        1.0f*size, 0.0f,      0.0f,      1.0f, 0.0f, 0.0f,  // x轴

                        0.0f,      0.0f,      0.0f,      0.0f, 1.0f, 0.0f,
                        0.0f,      1.0f*size, 0.0f,      0.0f, 1.0f, 0.0f,  // y轴

                        0.0f,      0.0f,      0.0f,      0.0f, 0.0f, 1.0f,
                        0.0f,      0.0f,      1.0f*size, 0.0f, 0.0f, 1.0f}; // Z轴

    unsigned int indices[] = {0, 1, 2, 3, 4, 5};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindVertexArray(1);
    
    mShader = std::make_unique<GLUtils::Shader>(vsShader, fsShader);
    mShader->use();
    bInited=true; // 控制析构函数删除缓存
}

void CoordinateAxis::Draw(Eigen::Matrix4f model, const Eigen::Matrix4f &projection, const Eigen::Matrix4f &viewMatrix)
{
    assert(bInited);

    mShader->use();
    mShader->set("model", model);
    mShader->set("view", viewMatrix);
    mShader->set("projection", projection);

    glBindVertexArray(VAO);
    glLineWidth(10);
    glDrawArrays(GL_LINES, 0, 6);
}