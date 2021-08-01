#include "obstacles.hpp"
#include <cppitertools/itertools.hpp>
#include <glm/gtx/rotate_vector.hpp>


void Obstacles::setupObstacles(){
    obstaclePositionList.at(0) = glm::vec3(1.0f, 0.0f, 0.0f);
    obstaclePositionList.at(1) = glm::vec3(0.0f, 1.0f, 0.0f);
    obstaclePositionList.at(2) = glm::vec3(-1.0f, 0.0f, 0.0f);
    obstaclePositionList.at(3) = glm::vec3(0.0f, -1.0f, 0.0f);

    obstaclePositionList.at(4) = glm::vec3(0.0f, 0.0f, 0.75f);
    obstaclePositionList.at(5) = glm::vec3(0.0f, 0.75f, 0.0f);
    obstaclePositionList.at(6) = glm::vec3(0.0f, 0.0f, -0.75f);
    obstaclePositionList.at(7) = glm::vec3(0.0f, -0.75f, 0.0f);

    obstaclePositionList.at(8) = glm::vec3(0.25f, 0.25f, 0.25f);
    obstaclePositionList.at(9) = glm::vec3(0.25f, 0.25f, -0.25f);
    obstaclePositionList.at(10) = glm::vec3(-0.25f, -0.25f, -0.25f);
    obstaclePositionList.at(11) = glm::vec3(-0.25f, -0.25f, 0.25f);
}

void Obstacles::update(float deltaTime){
    glm::vec3 rotationAxis1 = glm::cross(obstaclePositionList.at(0), obstaclePositionList.at(1));
    glm::vec3 rotationAxis2 = glm::vec3(0.0f, 1.0f, 0.0f);

    for (const auto index : iter::range(4)) {
        obstaclePositionList.at(index) = glm::rotate(obstaclePositionList.at(index), deltaTime*rotationAngle11, rotationAxis1);
        obstaclePositionList.at(index) = glm::rotate(obstaclePositionList.at(index), deltaTime*rotationAngle12, rotationAxis2);
    }

    rotationAxis1 = glm::cross(obstaclePositionList.at(4), obstaclePositionList.at(5));

    for (const auto index : iter::range(4, 8)) {
        obstaclePositionList.at(index) = glm::rotate(obstaclePositionList.at(index), -deltaTime*rotationAngle21, rotationAxis1);
        obstaclePositionList.at(index) = glm::rotate(obstaclePositionList.at(index), -deltaTime*rotationAngle22, rotationAxis2);
    }

    rotationAxis1 = glm::cross(obstaclePositionList.at(8), obstaclePositionList.at(9));
    rotationAxis2 = obstaclePositionList.at(8);

    for (const auto index : iter::range(8, 12)) {
        obstaclePositionList.at(index) = glm::rotate(obstaclePositionList.at(index), deltaTime*rotationAngle31, rotationAxis1);
        obstaclePositionList.at(index) = glm::rotate(obstaclePositionList.at(index), deltaTime*rotationAngle32, rotationAxis2);
    }
}