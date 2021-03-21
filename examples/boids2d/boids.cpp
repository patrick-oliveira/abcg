#include "boids.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

void Boids::terminateGL() {
    for (auto boid : m_boids) {
        glDeleteBuffers(1, &boid.m_vbo);
        glDeleteVertexArrays(1, &boid.m_vao);
    }
    
}

Boids::Boid Boids::createBoid(const GlobalData &globalData, Obstacles *obstacles){
    Boid boid;
    // Cada boid eh inicializado em uma posicao arbitraria, com uma velocidade/direcao inicial arbitraria
    glm::vec2 direction{m_randomDist(m_randomEngine), m_randomDist(m_randomEngine)};
    boid.m_velocity = globalData.speed*glm::normalize(direction);

    // os boids nao podem ser inicializados no interior de um obstaculo, entao aqui eh feita uma verificacao
    // percorrendo todos os obstaculos ja carregados
    int k;
    do{
        boid.m_translation = {m_randomDist(m_randomEngine), m_randomDist(m_randomEngine)};
        k = 0;
        for (auto &obstacle : (*obstacles).m_obstacles) {
            if(glm::distance(boid.m_translation, obstacle.m_translation) < boid.m_scale + 1.5*obstacle.m_scale)
                k = 1;
        }
    } while(k);

    boid.m_position.x = (globalData.viewportWidth/2)*boid.m_translation.x;
    boid.m_position.y = (globalData.viewportHeight/2)*boid.m_translation.y;

    // A rotacao tambem eh ajustada em funcao da velocidade inicial para que os boids sempre apontem para onde
    // estao andando
    boid.m_rotation = std::atan2(boid.m_velocity.y, boid.m_velocity.x) - M_PI_2;

    std::vector<glm::vec2> positions(0);

    positions.emplace_back( 0,  0);
    positions.emplace_back( 0,  1);
    positions.emplace_back( 0.7, -1);
    positions.emplace_back(-0.7, -1);
    positions.push_back(positions.at(1));

    glGenBuffers(1, &boid.m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, boid.m_vbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2), positions.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

    glGenVertexArrays(1, &boid.m_vao);

    glBindVertexArray(boid.m_vao);

    glEnableVertexAttribArray(positionAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, boid.m_vbo);
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    return boid;
}

void Boids::initializeGL(GLuint program, int quantity, const GlobalData &globalData, Obstacles *obstacles) {
    terminateGL();

    auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
    m_randomEngine.seed(seed);

    m_program = program;
    m_colorLoc       = glGetUniformLocation(m_program, "color");
    m_rotationLoc    = glGetUniformLocation(m_program, "rotation");
    m_scaleLoc       = glGetUniformLocation(m_program, "scale");
    m_translationLoc = glGetUniformLocation(m_program, "translation");

    m_boids.clear();
    m_boids.resize(quantity);

    for (auto &boid : m_boids) {
        boid = createBoid(globalData, obstacles);
    }
}

void Boids::paintGL() {
    glUseProgram(m_program);

    for (auto &boid : m_boids) {
        glBindVertexArray(boid.m_vao);
        glUniform4f(m_colorLoc, 1, 1, 1, 1);
        glUniform1f(m_rotationLoc, boid.m_rotation);
        glUniform1f(m_scaleLoc, boid.m_scale);
        glUniform2f(m_translationLoc, boid.m_translation.x, boid.m_translation.y);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
        glBindVertexArray(0);
    }

    glUseProgram(0);
}

void Boids::update(const GlobalData &globalData, glm::vec2 predatorLoc, Obstacles *obstacles, float deltaTime) {
    for (auto &boid : m_boids) {
        // Chama as funcoes que governam o comportamento de floculacao
        cluster(&boid);
        avoid(&boid);
        match(&boid);
        speedLimit(&boid);  
        
        // Se o predator estiver ativado, eh verificado se o predator esta proximo do boid, 
        // e se for o caso, o vetor velocidade sofre uma alteracao na direcao contraria a posicao do predador
        if(globalData.m_removePredator == false) {
            float distance = glm::length(boid.m_translation - predatorLoc);
            if (distance < m_minPredatorDistance){
                boid.m_velocity.x += (boid.m_translation.x - predatorLoc.x)*m_predatorAvoidFactor;
                boid.m_velocity.y += (boid.m_translation.y - predatorLoc.y)*m_predatorAvoidFactor;
            }
        }
        
        // Eh verificado se o boid esta dentro do escopo da janela da aplicacao
        // Como a velocidade/ direcao eh ajustada gradativamente, essa verificacao
        // comeca ja dentro de uma margem, assim os boids geralmente nao sairao da janela,
        // e se sairem (devido a velocidade acumulada), voltarao rapidamente
        if (boid.m_translation.x > 1.0f - globalData.margin)
            boid.m_velocity.x -= m_speedLimit*globalData.correction;
        if (boid.m_translation.y > 1.0f - globalData.margin)
            boid.m_velocity.y -= m_speedLimit*globalData.correction;
        if (boid.m_translation.x < -1.0f + globalData.margin)
            boid.m_velocity.x += m_speedLimit*globalData.correction;
        if (boid.m_translation.y < -1.0f + globalData.margin)
            boid.m_velocity.y += m_speedLimit*globalData.correction;

        // Eh verificado se o boid esta colidindo com um obstaculo, de maneira similar ao que foi feito
        // no exercicio de asteroids.
        // Caso haja colisao, a velocidade do boid eh ajustada por uma rotacao para a direcao do vetor
        // tangente ao obstaculo, assim o boid consegue desviar para os lados
        for (auto & obstacle : (*obstacles).m_obstacles) {
            if (glm::distance(boid.m_translation, obstacle.m_translation) < boid.m_scale + obstacle.m_scale){
                glm::vec2 tangentVector = {obstacle.m_translation.y - boid.m_translation.y,
                                           boid.m_translation.x - obstacle.m_translation.x};
                float angle = -0.8*glm::angle(tangentVector, boid.m_velocity); // o termo 0.8 foi obtido empiricamente, para que o
                                                                               // ajuste seja mais suave
                boid.m_velocity = glm::rotate(boid.m_velocity, angle);
            }
        }  
            
        boid.m_translation += (globalData.speed*boid.m_velocity)*deltaTime;
        boid.m_rotation = std::atan2(boid.m_velocity.y, boid.m_velocity.x) - M_PI_2;

        boid.m_position.x = (globalData.viewportWidth/2)*boid.m_translation.x;
        boid.m_position.y = (globalData.viewportHeight/2)*boid.m_translation.y;
    }
}

void Boids::speedLimit(Boid *boid) {
    // mantem a velocidade do boid dentro de um limite (em modulo)
    float speed = glm::length((*boid).m_velocity);
    if(speed > m_speedLimit) {
        (*boid).m_velocity = glm::normalize((*boid).m_velocity)*m_speedLimit;
    }
}

float Boids::distance(Boid *b1, Boid *b2) {
    // calcula a distancia entre dois boids
    return  glm::length((*b1).m_translation - (*b2).m_translation);
}

void Boids::cluster(Boid *boid) {
    // calcula o centro de gravidade de todos os boinds vizinhos suficientemente proximos
    glm::vec2 center{0.0f, 0.0f};

    int N = 0;
    for (auto &b1 : m_boids) {
        if (boid != &b1 && distance(boid, &b1) < m_visualRange){
            center += b1.m_translation;
            N += 1;
        }
    }
    
    if (N > 0) {
        center /= N;
        (*boid).m_velocity.x += (center.x - (*boid).m_translation.x) * m_centeringFactor;
        (*boid).m_velocity.y += (center.y - (*boid).m_translation.y) * m_centeringFactor;
    }
}

void Boids::avoid(Boid *boid){
    // Recebe um ajuste na direcao contraria aos boids vizinhos proximos de mais
    for (auto &b1 : m_boids) {
        if (boid != &b1 && distance(boid, &b1) < m_minDistance){
            (*boid).m_velocity.x += ((*boid).m_translation.x - b1.m_translation.x)*m_avoidFactor;
            (*boid).m_velocity.y += ((*boid).m_translation.y - b1.m_translation.y)*m_avoidFactor;
        }
    }

    
}

void Boids::match(Boid *boid){
    // Eh feito um ajuste para que o boid entre em sincronia com a direcao do seu grupo, i.e.
    // os vizinhos suficientemente proximos
    glm::vec2 avgVelocity{0.0f, 0.0f};
    int N{0};

    for (auto &b1 : m_boids) {
        if (boid != &b1 && distance(boid, &b1) < m_visualRange) {
            avgVelocity.x += b1.m_velocity.x;
            avgVelocity.y += b1.m_velocity.y;
            N +=1;
        }
    }

    if (N > 0){
        avgVelocity.x += (avgVelocity.x)/N;
        avgVelocity.y += (avgVelocity.y)/N;
        (*boid).m_velocity.x += (avgVelocity.x - (*boid).m_velocity.x) * m_matchingFactor;
        (*boid).m_velocity.y += (avgVelocity.y - (*boid).m_velocity.y) * m_matchingFactor;
    }
}