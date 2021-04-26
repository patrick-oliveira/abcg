#ifndef BOIDS_HPP_
#define BOIDS_HPP_

#include <glm/vec3.hpp>
#include <list>
#include <random>
#include "abcg.hpp"
#include "obstacles.hpp"
#include "lightsource.hpp"

class OpenGLWindow;
class Obstacles;
class LightSource;

class Boids {
    public:
        void update(float deltaTime, Obstacles *obstacles, LightSource *lightsource);

    private:
        friend OpenGLWindow;

        float boxLimit{1.5f};
        float margin{0.05f};

        std::default_random_engine m_randomEngine;
        std::uniform_real_distribution<float> m_randomDist{-boxLimit, boxLimit};

        struct Boid {
            glm::vec3 position{glm::vec3(0.0f, 0.0f, 0.0f)};
            glm::vec3 velocity{glm::vec3(0.0f, 0.0f, 1.0f)};
            
            glm::vec3 rotationAxis;
            float angle;
        };
        
        float scale{0.05f};

        int num_boids{300};
        std::array<Boid, 300> boid_list;

        float visualRange{0.5f};
        float speedFactor{0.9f};
        float speedLimit{0.8f};
        float minDistance{0.1f};
        float centeringFactor{0.1f};
        float avoidanceFactor{0.15f};
        float matchingFactor{0.01f};
        float correctionFactor{0.1f};

        Boids::Boid createBoid();
        void createBoids();
        void updateOrientation(Boid *boid);
        void updatePosition(Boid *boid, glm::vec3 translation);
        float distance(Boid *b1, Boid *b2);
        void cluster(Boid *boid);
        void avoid(Boid *boid);
        void match(Boid *boid);
        void limitSpeed(Boid *boid);
        void limitPosition(Boid *boid);
        void avoidObstacles(Boid *boid, Obstacles *obstacles, LightSource *lightsource);
};

#endif

