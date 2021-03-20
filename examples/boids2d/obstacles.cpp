#include "obstacles.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void Obstacles::initializeGL(GLuint program, int quantity) {
    terminateGL();

    auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
    m_randomEngine.seed(seed);

    m_program = program;
    m_colorLoc = glGetUniformLocation(m_program, "color");
    m_scaleLoc = glGetUniformLocation(m_program, "scale");
    m_translationLoc = glGetUniformLocation(m_program, "translation");

    m_obstacles.clear();
    m_obstacles.resize(quantity);

    for (auto &obstacle : m_obstacles) {
        obstacle = createObstacle();
    }
}

void Obstacles::paintGL() {
    glUseProgram(m_program);

    for (auto &obstacle : m_obstacles) {
        glBindVertexArray(obstacle.m_vao);

        glUniform4fv(m_colorLoc, 1, &obstacle.m_color.r);
        glUniform1f(m_scaleLoc, obstacle.m_scale);
        glUniform2f(m_translationLoc, obstacle.m_translation.x, obstacle.m_translation.y);

        glDrawArrays(GL_TRIANGLE_FAN, 0, obstacle.m_sides + 2);

        glBindVertexArray(0);
    }

    glUseProgram(0);
}

void Obstacles::terminateGL() {
    for(auto obstacle : m_obstacles) {
        glDeleteBuffers(1, &obstacle.m_vbo);
        glDeleteVertexArrays(1, &obstacle.m_vao);
    }
}

Obstacles::Obstacle Obstacles::createObstacle() {
    Obstacle obstacle;

    auto &re{m_randomEngine};

    // Numero de lados eh aleatorio
    std::uniform_int_distribution<int> randomSides(16, 40);
    obstacle.m_sides = randomSides(re);

    // Cor (tom de cinza) eh aleatoria
    std::uniform_real_distribution<float> randomIntensity(0.5f, 1.0f);
    obstacle.m_color = glm::vec4(1) * randomIntensity(re);
    obstacle.m_color.a = 1.0f;

    // Fator de escala eh aleatorio
    std::uniform_real_distribution<float> rd2(0.1f, 0.3f);
    auto scale{rd2(re)};
    obstacle.m_scale = scale;

    // A posicao onde o obstaculo ficara eh aleatoria, porem ele nao pode ficar nem tao perto do centro, onde o predador inicialmente aparece,
    // e nem muito proximo das bordas, pois pode induzir os passaros a sair da tela para conseguir desviar dos obstaculos.
    std::uniform_real_distribution<float> rd1(-1.0f, 1.0f);
    float length;
    do {
        obstacle.m_translation = {rd1(re), rd1(re)};
        length = glm::length(obstacle.m_translation);
    } while (length < 0.2f || length > 0.9f);


    // Draw polygon
    std::vector<glm::vec2> positions(0);
    positions.emplace_back(0, 0);
    auto step{M_PI*2 / obstacle.m_sides};
    std::uniform_real_distribution<float> randomRadius(0.8f, 1.0f);
    for (auto angle : iter::range(0.0, M_PI * 2, step)) {
        auto radius{randomRadius(re)};
        positions.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
    }
    positions.push_back(positions.at(1));

    glGenBuffers(1, &obstacle.m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, obstacle.m_vbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2), positions.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

    glGenVertexArrays(1, &obstacle.m_vao);
    glBindVertexArray(obstacle.m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, obstacle.m_vbo);
    glEnableVertexAttribArray(positionAttribute);
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    return obstacle;

}

