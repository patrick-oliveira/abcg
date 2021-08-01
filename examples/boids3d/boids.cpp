#include "boids.hpp"
#include <cppitertools/itertools.hpp>
#include <glm/gtx/rotate_vector.hpp>

Boids::Boid Boids::createBoid() {
    Boid new_boid;

    glm::vec3 newDirection{glm::vec3(m_randomDist(m_randomEngine),
                                    m_randomDist(m_randomEngine),
                                    m_randomDist(m_randomEngine))};
    // glm::vec3 newDirection{glm::vec3(-1.0f, 0, 0)};
    // newDirection = glm::normalize(newDirection);
    new_boid.velocity = newDirection;
    updateOrientation(&new_boid);

    glm::vec3 newPosition = glm::vec3{m_randomDist(m_randomEngine),
                                      m_randomDist(m_randomEngine),
                                      m_randomDist(m_randomEngine)};
    updatePosition(&new_boid, newPosition);

    return new_boid;
}

void Boids::createBoids(){
    for (const auto index : iter::range(num_boids)) {
        boid_list.at(index) = createBoid();
    }
}

void Boids::updateOrientation(Boid *boid){
    // Rotaciona o 
    glm::vec3 originalDirection{glm::vec3(0.0f, 0.0f, 1.0f)};
    glm::vec3 currentDirection = glm::normalize((*boid).velocity);

    (*boid).rotationAxis = glm::cross(originalDirection, currentDirection);
    (*boid).angle = glm::acos(glm::dot(originalDirection, currentDirection));
}

void Boids::updatePosition(Boid *boid, glm::vec3 translation) {
    (*boid).position = (*boid).position + translation;
}

float Boids::distance(Boid *b1, Boid *b2){
    return glm::length((*b1).position - (*b2).position);
}

void Boids::cluster(Boid *boid){
    glm::vec3 center{0.0f, 0.0f, 0.0f};

    int N = 0;
    for (const auto index : iter::range(num_boids)){
        auto &neighbour{boid_list.at(index)};
        if(boid != &neighbour && distance(boid, &neighbour) < visualRange){
            center.x += neighbour.position.x;
            center.y += neighbour.position.y;
            center.z += neighbour.position.z;
            N += 1;
        }
    }

    if (N > 0){
        center /= N;
        (*boid).velocity.x += (center.x - (*boid).position.x) * centeringFactor;
        (*boid).velocity.y += (center.y - (*boid).position.y) * centeringFactor;
        (*boid).velocity.z += (center.z - (*boid).position.z) * centeringFactor;
    }
}

void Boids::avoid(Boid *boid){
    for (const auto index : iter::range(num_boids)) {
        auto &neighbour{boid_list.at(index)};
        if (boid != &neighbour && distance(boid, &neighbour) < minDistance){
            (*boid).velocity.x += ((*boid).position.x - neighbour.position.x) * avoidanceFactor;
            (*boid).velocity.y += ((*boid).position.y - neighbour.position.y) * avoidanceFactor;
            (*boid).velocity.z += ((*boid).position.z - neighbour.position.z) * avoidanceFactor;
        }
    }
}

void Boids::match(Boid *boid){
    glm::vec3 averageVelocity{0.0f, 0.0f, 0.0f};
    int N = 0;

    for (const auto index : iter::range(num_boids)) {
        auto &neighbour{boid_list.at(index)};
        if (boid != &neighbour && distance(boid, &neighbour) < visualRange) {
            averageVelocity.x += neighbour.velocity.x;
            averageVelocity.y += neighbour.velocity.y;
            averageVelocity.z += neighbour.velocity.z;
            N += 1;
        }
    }

    if (N > 0) {
        averageVelocity /= N;
        (*boid).velocity.x += (averageVelocity.x - (*boid).velocity.x) * matchingFactor;
        (*boid).velocity.y += (averageVelocity.y - (*boid).velocity.y) * matchingFactor;
        (*boid).velocity.z += (averageVelocity.z - (*boid).velocity.z) * matchingFactor;
    }
}

void Boids::avoidObstacles(Boid *boid, Obstacles *obstacles, LightSource *lightsource){
    for (const auto index : iter::range((*obstacles).num_obstacles)){
        glm::vec3 obstaclePosition = (*obstacles).obstaclePositionList.at(index);

        if (glm::distance((*boid).position, obstaclePosition) < visualRange){
            (*boid).velocity.x += ((*boid).position.x - obstaclePosition.x) * correctionFactor;
            (*boid).velocity.y += ((*boid).position.y - obstaclePosition.y) * correctionFactor;
            (*boid).velocity.z += ((*boid).position.z - obstaclePosition.z) * correctionFactor;
        }
    }

    if (glm::distance((*boid).position, (*lightsource).lightSourcePosition) < visualRange + 0.1){
        (*boid).velocity.x += ((*boid).position.x - (*lightsource).lightSourcePosition.x) * 0.2;
        (*boid).velocity.y += ((*boid).position.y - (*lightsource).lightSourcePosition.y) * 0.2;
        (*boid).velocity.z += ((*boid).position.z - (*lightsource).lightSourcePosition.z) * 0.2;
    }
}

void Boids::limitSpeed(Boid *boid){
    float speed = glm::length((*boid).velocity);
    
    if (speed > speedLimit) {
        (*boid).velocity = glm::normalize((*boid).velocity) * speedLimit;
    }
}

void Boids::limitPosition(Boid *boid){
    if ((*boid).position.x >= boxLimit - margin)
        (*boid).velocity.x -= correctionFactor;
    if ((*boid).position.x <= -boxLimit + margin)
        (*boid).velocity.x += correctionFactor;

    if ((*boid).position.y >= boxLimit - margin)
        (*boid).velocity.y -= correctionFactor;
    if ((*boid).position.y <= -boxLimit + margin)
        (*boid).velocity.y += correctionFactor;

    if ((*boid).position.z >= boxLimit - margin)
        (*boid).velocity.z -= correctionFactor;
    if((*boid).position.z <= -boxLimit + margin)
        (*boid).velocity.z += correctionFactor;
}

void Boids::update(float deltaTime, Obstacles *obstacles, LightSource *lightsource){
    for (const auto index : iter::range(num_boids)){
        auto &boid{boid_list.at(index)};
        cluster(&boid);
        avoid(&boid);
        match(&boid);
        limitPosition(&boid);
        avoidObstacles(&boid, obstacles, lightsource);
        limitSpeed(&boid);

        
        updateOrientation(&boid);
        updatePosition(&boid, deltaTime*(speedFactor*boid.velocity));
    }
}

