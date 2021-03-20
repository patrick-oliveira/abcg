#ifndef BOIDS_HPP_
#define BOIDS_HPP_

#include <list>
#include <random>

#include "abcg.hpp"
#include "globaldata.hpp"
#include "obstacles.hpp"

class OpenGLWindow;
class Obstacles;

class Boids {
    public:
        void initializeGL(GLuint program, int quantity, const GlobalData &globalData, Obstacles *obstacles);
        void paintGL();
        void terminateGL();
        void update(const GlobalData &globalData, glm::vec2 predatorLoc, Obstacles *obstacles, float deltaTime);

    private:
        friend OpenGLWindow;

        GLuint m_program{};
        GLint m_colorLoc{};
        GLint m_rotationLoc{};
        GLint m_translationLoc{};
        GLint m_scaleLoc{};

        struct Boid {
            GLuint m_vao{};
            GLuint m_vbo{};

            glm::vec4 m_color{1};
            float m_scale{0.007f};
            float m_rotation{};
            glm::vec2 m_translation{glm::vec2(0)};
            glm::vec2 m_position{glm::vec2(0)};
            glm::vec2 m_velocity{glm::vec2(0)};
        };

        std::list<Boid> m_boids;
        
        float m_visualRange{0.2f};
        float m_speedLimit{0.9f};
        float m_minDistance{0.1f};
        float m_minPredatorDistance{0.2f};

        float m_centeringFactor{0.005f}; // Coloque isso em um slider
        float m_avoidFactor{0.5f};
        float m_predatorAvoidFactor{2.0f};
        float m_matchingFactor{0.05f};

        std::default_random_engine m_randomEngine;
        std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};

        Boids::Boid createBoid(const GlobalData &globalData, Obstacles *obstacles);
        float distance(Boid *b1, Boid *b2);
        void cluster(Boid *boid);
        void avoid(Boid *boid);
        void match(Boid *boid);
        void speedLimit(Boid *boid);
};

#endif