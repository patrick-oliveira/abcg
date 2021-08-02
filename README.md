# Boids 2D/3D

Obs: Projeto elaborado no contexto da disciplina de Computação Gráfica, na UFABC.

O repositório é um fork do repositório original da disciplina, contendo a biblioteca ABCg que facilita a prototipagem de aplicações gráficas interativas 3D em C++ capazes de rodar tanto no desktop quanto no navegador, sem alteração do código-fonte. O código das aplicações estão contidas na pasta _"examples"_.

As simulações podem ser visualizadas nos links
- [Boids 2d](https://patrick-oliveira.github.io/Boids2Dwebgl/boids2d)
- [Boids 3d](https://patrick-oliveira.github.io/Boids3Dwebgl/)

## Descrição do Programa

### Modelo

_Boids_ é um tipo de simulação baseada em agentes. Este tipo de modelo computacional envolve um conjunto de agentes autônomos que se comportam e interagem de acordo com regras fundamentais pré-programadas, e a simulação objetiva observar o comportamento global do sistema e quais resultados são possíveis de ocorrer a partir destas regras fundamentais.

O sistema consiste em um grupo de agentes (boids) munidos dos atributos de posição, velocidade escalar e orientação. Cada agente age segundo três regras:

1. Separação: tendência de se manter a uma certa distância dos boids vizinhos.

A função desta regra é fazer com que os boids não colidam uns com os outros, se concentrando em um mesmo ponto da tela. A regra depende de um parâmetro que estabelece uma distância mínima. Cada boid b1 em uma posição P1 verifica se algum vizinho b2 na posição P2 está a uma distância menor que a mínima, e sendo este o caso, b1 faz um deslocamento na direção P1 - P2.

2. Coesão: tendência de se mover para a posição média do grupo de boids mais próximo.

A função desta regra é induzir (em grau ajustável) os boids a formarem grupos de agentes. Para isso, cada boid em uma posição P verifica a posição média percebida do grupo de boids vizinhos (dentro de um raio determinado), denotada por Pm, e sua orientação sofre um ajuste na direção Pm - P.

3. Alinhamento: tendência de se mover segundo a orientação média dos boids mais próximos.

Esta regra contribui para que os boids apresentem comportamento grupal, movendo-se em uma mesma direção uma vez que formaram um grupo. Cada boid b1 de orientação v1 tem sua orientação alterada no sentido da orientação média dos boids vizinhos, próximos a uma distância determinada.

### Recursos

#### 1. Boids 2D

Para incluir no sistema a possibilidade de input de informação do mouse, é possível incluir um predador com o botão esquerdo e que se moverá na direção apontada na direção do mouse. Todos os outros boids reagirão à presença do predador, se afastando com um grau ajustável. Os agentes possuem a capacidade de desviar de obstáculos que são adicionados aleatoriamente durante a inicialização do sistema.

O modelo contém oito parâmetros ajustáveis por sliders:

1. Raio de Visão : \[0.0, 1.0\]. Determina o raio do círculo centrado em cada boid e que delimita o seu alcance visual.
2. Limite de Velocidade : \[0.0, 3.0\]. Determina a velocidade escalar máxima dos boids.
3. Distância Mínima de Separação : \[0.0, 0.3\]. Determina a distância mínima a ser mantida entra qualquer par de boids.
4. Distância Mínima do Predador : \[0.0, 0.3\]. Determina a menor distância a ser mantida entre os boids e o predador.
5. Fator de Centralidade : \[0.0, 1.0\]. Parâmetro que determina o grau com que os boids se movem na direção da posição média do grupo mais próximo.
6. Fator de Separação : \[0.0, 3.0\]. Parâmetro que determina o grau de repulsão entre dois boids mais próximos que a distância mínima permitida.
7. Fator de Separação do Predador : \[0.0, 3.0\]. Parâmetro que determina o grau de repulsão entre os boids e o predador, caso estejam a uma distância menor que a mínima permitida.
8. Coeficiente de Alinhamento: Determina o grau com que os boids assumirão a orientação média do grupo mais próximo.

Estes parâmetros podem ser ajustados para observar mudanças no comportamento global dos boids. Por exemplo, um ângulo visual menor fará com que os boids se organizem em grupos pequenos; um fator de centralidade alto fará com que os grupos sejam coerentes e não se dispersem facilmente, e de modo geral os boids terminarão formando um único grupo (que pode ser perturbado pelo predador); um limite de velocidade elevado contribui para a desagregação dos grupos; um fator de separação elevado fará com que os boids se espalhem pelo cenário, não necessariamente se desagregando.

#### 3. Boids 3D



## Objetivos Futuros

### Otimização

## Referências

A aplicação é baseada no modelo clássico e mais básico proposto por Craig W. Raynolds. Referências em pseudocódigo podem ser encontradas em \[1\] e a página do criador do modelo está em \[2\].

- \[1\]: http://www.kfish.org/boids/pseudocode.html
- \[2\]: http://www.red3d.com/cwr/boids/

