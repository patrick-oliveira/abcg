#ifndef OBSTACLES_HPP_
#define OBSTACLES_HPP_

#include <list>
#include <random>

#include "abcg.hpp"
#include "predator.hpp"
#include "boids.hpp"

class OpenGLWindow;
class Predator;
class Boids;

class Obstacles {
    // Os obstaculos sao construidos de maneira similar ao modo como foram construidos os asteroides e os poligonos nos trabalhos anteriores
    public:
        void initializeGL(GLuint program, int quantity);
        void paintGL();
        void terminateGL();

    private:
        friend OpenGLWindow;
        friend Predator;
        friend Boids;

        GLuint m_program{};
        GLint m_colorLoc{};
        GLint m_translationLoc{};
        GLint m_scaleLoc{};

        struct Obstacle {
            GLuint m_vao{};
            GLuint m_vbo{};

            glm::vec4 m_color{1};
            int m_sides{};
            float m_scale{};
            glm::vec2 m_translation{glm::vec2(0)};
        };

        std::list<Obstacle> m_obstacles;
        std::default_random_engine m_randomEngine;
        std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};

        Obstacles::Obstacle createObstacle();
};

#endif