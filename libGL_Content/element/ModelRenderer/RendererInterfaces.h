#pragma once

#include <string>
#include <opencv2/core.hpp>

namespace GLRenderer
{
    class MeshRendererInterface
    {
        public:
            // 构造函数
            MeshRendererInterface(int width, int height, const std::string &folder):m_width(width), m_height(height), m_folder(folder)
            {
                InitGL();
            }

            // 析构函数
            virtual ~MeshRendererInterface()
            {
                glDeleteFramebuffers(1,&framebuffer);
                glDeleteRenderbuffers(1, &rbo);
                glDeleteTextures(1,&textureColorbuffer);
            }

            // 渲染
            virtual void Render(const Eigen::Matrix4f &projection, const Eigen::Matrix4f &view, float near, float far)=0

            cv::Mat GetRGB() const {return m_rgb;}
            cv::Mat GetDepth() const {return m_depth;}

        private:
            int m_width, m_height;
            std::string m_folder;
            cv::Mat m_rgb, m_depth;
            GLuint framebuffer, textureColorbuffer, rbo;

            // 初始化OpenGL
            void InitGL()
            {
                // Init GL buffers
                glGenFramebuffers(1, &framebuffer);
                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

                glGenTextures(1, &textureColorbuffer);
                glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

                // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
                glGenRenderbuffers(1, &rbo);
                glBindRenderbuffer(GL_RENDERBUFFER, rbo);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, m_width, m_height); // use a single renderbuffer object for both a depth AND stencil buffer.
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

                if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }
    }
}