#ifndef GLOBALDATA_HPP_
#define GLOBALDATA_HPP_

struct GlobalData{
    // Guarda alguns parametros utilizados pelos boids e pelo predador
    float margin{0.15f}; // impoe uma margem na tela, delimitando um espaco onde as direcoes dos elementos
                         // da tela se ajustam gradativamente ate que retornem para o centro da tela
    float correction{0.2f}; // esse ajuste depende deste parametro de intensidade
    
    float speed{0.30f};     // limita a velocidade com que o predador vai percorrer a tela
    
    float viewportWidth;    // guarda uma copia destes parametros para pegar o vetor de posicao nao-normalizado
    float viewportHeight;   // dos elementos da tela
    
    bool m_removePredator{true}; // Parametro de verificacao da remocao do predador do modelo
};

#endif