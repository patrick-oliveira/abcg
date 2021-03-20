#ifndef PREDATOR_HPP_
#define PREDATOR_HPP_

#include "abcg.hpp"
#include "globaldata.hpp"
#include "obstacles.hpp"
#include <iostream>
#include <string>

class OpenGLWindow;
class Obstacles;

class Predator {
    public:
        void initializeGL(GLuint program);
        void paintGL();
        void terminateGL();

        void update(const GlobalData &globalData, Obstacles *obstacles, float deltaTime);

    private:
        friend OpenGLWindow;

        GLuint m_program{};
        GLint m_colorLoc{};
        GLint m_rotationLoc{};
        GLint m_translationLoc{};
        GLint m_scaleLoc{};

        GLuint m_vao{};
        GLuint m_vbo{};

        float m_rotation{0.0f};
        float m_scale{0.01f};
        glm::vec2 m_translation{glm::vec2(0)};
        glm::vec2 m_position{glm::vec2(0)};
        glm::vec2 m_target{glm::vec2(0)};
        glm::vec2 m_direction{glm::vec2(0)};
        glm::vec2 m_velocity{glm::vec2(0)};

        std::string output;
};

#endif

