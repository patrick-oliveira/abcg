#include "lightsource.hpp"
#include <glm/gtx/rotate_vector.hpp>

void LightSource::setupLightSource(){
    lightSourcePosition = glm::vec3(0.0f, 1.5f, 0.0f);
}

void LightSource::update(float deltaTime) {
    // glm::vec3 axis1 = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 axis2 = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 axis3 = glm::vec3(0.0f, 0.0f, 1.0f);

    // lightSourcePosition = glm::rotate(lightSourcePosition, deltaTime * rotationAngle, axis1);
    lightSourcePosition = glm::rotate(lightSourcePosition, deltaTime * rotationAngle, axis2);
    lightSourcePosition = glm::rotate(lightSourcePosition, deltaTime * rotationAngle, axis3);
}