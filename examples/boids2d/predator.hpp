#ifndef PREDATOR_HPP_
#define PREDATOR_HPP_

#include "abcg.hpp"
#include "globaldata.hpp"
#include "obstacles.hpp"

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
        glm::vec2 m_translation{glm::vec2(0)};  // da as coordenadas dentro de [-1.0, 1.0] x [-1.0, 1.0]
        glm::vec2 m_position{glm::vec2(0)};     // da as coordenadas em termos do tamanho da janela, em pixel
        glm::vec2 m_target{glm::vec2(0)};       // recebe o "alvo" que eh o local apontado pelo mouse
        glm::vec2 m_direction{glm::vec2(0)};    // eh um vetor normal que especifica a direcao para onde o predador precisa caminahr
                                                // esse vetor eh utilizado tanto para dar a velocidade quanto para dar a rotacao do desenho
        glm::vec2 m_velocity{glm::vec2(0)};     // este vetor tera a mesma direcao de m_direction, mas seu modulo sofre alteracoes de outros parametros
};

#endif

