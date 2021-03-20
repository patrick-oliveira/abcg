#include "predator.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

void Predator::terminateGL() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void Predator::initializeGL(GLuint program) {
    m_position = glm::vec2{0.0f, 0.0f};
    m_target = glm::vec2{0.0f, 0.0f};
    m_direction = glm::vec2{0.0f, 1.0f};
    m_velocity = glm::vec2{0.0f, 1.0f};

    terminateGL();

    m_program = program;

    m_colorLoc       = glGetUniformLocation(m_program, "color");
    m_rotationLoc    = glGetUniformLocation(m_program, "rotation");
    m_scaleLoc       = glGetUniformLocation(m_program, "scale");
    m_translationLoc = glGetUniformLocation(m_program, "translation");

    std::vector<glm::vec2> positions(0);

    positions.emplace_back( 0,  0);
    positions.emplace_back( 0,  1);
    positions.emplace_back( 1, -1);
    positions.emplace_back(-1, -1);
    positions.push_back(positions.at(1));

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2), positions.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

    glGenVertexArrays(1, &m_vao);

    glBindVertexArray(m_vao);

    glEnableVertexAttribArray(positionAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Predator::paintGL() {
    glUseProgram(m_program);

    glBindVertexArray(m_vao);
    glUniform4f(m_colorLoc, 1, 0, 0, 1);
    glUniform1f(m_rotationLoc, m_rotation);
    glUniform1f(m_scaleLoc, m_scale);
    glUniform2f(m_translationLoc, m_translation.x, m_translation.y);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Predator::update(const GlobalData &globalData, Obstacles *obstacles, float deltaTime) {
    // Pega posicao do mouse constantemente para manter a direcao fixada
    glm::ivec2 mousePosition;
    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
    m_target = glm::vec2{mousePosition.x - globalData.viewportWidth/2,
                         mousePosition.y - globalData.viewportHeight/2};
    m_target.y = - m_target.y; // Isso vale para o sistema de coordenadas do mouse somente
    
    m_direction = glm::normalize(m_target - m_position);
    if (glm::length(m_direction) > 0.05)
        m_rotation = std::atan2(m_direction.y, m_direction.x) - M_PI_2;

    m_velocity = globalData.speed*m_direction;

    for (auto & obstacle : (*obstacles).m_obstacles) {
        if (glm::distance(m_translation, obstacle.m_translation) < m_scale + 1.1*obstacle.m_scale){
            glm::vec2 tangentVector = {obstacle.m_translation.y - m_translation.y,
                                        m_translation.x - obstacle.m_translation.x};
            float angle = -0.8*glm::angle(tangentVector, m_velocity);
            m_velocity = glm::rotate(m_velocity, angle);
        }
    }

    m_translation += m_velocity*deltaTime;

    m_position.x = (globalData.viewportWidth/2)*m_translation.x;
    m_position.y = (globalData.viewportHeight/2)*m_translation.y;
}
