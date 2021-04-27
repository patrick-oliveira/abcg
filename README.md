# Boids 3D
## Autoria
Patrick Oliveira de Paula - RA: 11002616

## Recursos do Programa

Este projeto é uma extensão do projeto Boids 2D, sendo adaptado para 3D. A dinâmica dos boids é fundamentalmente a mesma, com funções de agrupamento, afastamento e imitação para controlar o comportamento grupal dos boids. Além disso,

- Ao abrir, a aplicação vai "capturar" o mouse a fim de que seja possível controlar a direção da câmera com o mouse. Se necessário, para sair basta apertar o botão do windows. No WebGL essa função não funciona muito bem, o mouse não é muito preciso, mas na aplicação desktop funciona adequadamente. O deslocamento da câmera ao longo do espaço é controlado pelas teclas W A S D (frente, esquerda, trás, direita), Q e E (rotação para a esquerda e direita), I e K (rotação para cima e para baixo), não sendo estritamente necessário o mouse para controlar a direção da câmera. Por fim, ao clicar com o botão esquerdo do mouse o movimento de todos os elementos da tela são congelados; clicar novamente reativa a animação. Obs: ao abrir a aplicação pelo desktop, pode ser necessário clicar uma vez para que os comandos do teclado sejam lidos (e uma outra vez para reativar a animação, desativada com o primeiro clique).

- Os boids possuem a capacidade de desviar de obstáculos, e para isso foram inseridos 12 obstáculos esféricos (com texturas de rocha) que fazem rotações no espaço. O intuito com isso era fazer com que as rotações entrassem em sincronia, mas fazer esse tipo de animação (quero dizer, estabelecer a sincronia) mostrou-se mais trabalhoso que o esperado, então a animação não é tão bonita quanto eu gostaria que fosse. A presença dos obstáculos contribui para que os boids não se juntem em um grande grupo.

- Existe mais um outro obstáculo, uma esfera com textura de fogo, que especifica também a posição da fonte de luz da cena.

- Apesar dos obstáculos e da fonte de luz utilizarem textura, os boids utilizam apenas iluminação de Blinn-Phong. Em parte por decisão estética, mas mantive os dois shaders na pasta. Os obstáculos foram gerados pelo software "3D Builder" utilizando imagens obtidas na internet para as texturas. 

- Os parâmetros de controle do comportamento dos boids permanece o mesmo, mas agora eles são ajustados em 0.1 pelo teclado. A seguir, os parâmetros serão descritos, juntamente com as teclas de acréscimo e de decréscimo de 0.1.

- O modelo envolve 8 parâmetros ajustáveis por "Sliders":
  - __Exemplo\[Tecla de Acréscimo - Tecla de Decréscimo\]: Função__
  - Visual Range\[F1 - 1\] : Determina o raio do círculo centrado em cada pássaro e que delimita o seu círculo de visão. Para um pássaro A, todo pássaro B que estiver dentro do círculo de visão será incluído nos cálculos de atualização da direção de A.
  - Speed Factor\[F2 - 2\]  : Determina a velocidade dos pássaros. A velocidade contribui para determinar a coerência dos pássaros pois quanto maior a velocidade, mais facilmente cada pássaro escapa dos grupos que se formam espontaneamente.
  - Min Distance\[F3 - 3\] : Determina a menor distância que pode haver entre qualquer par de pássaros. Quando dois pássaros A e B estão distanciados por este limiar (ou menos), sua velocidade é incrementada nas direções mutuamente opostas, a fim de que se afastem, com intensidade a depender do parâmetro "Avoid Factor".
  - Centering Factor \[F4 - 4\]  : É o parâmetro utilizado para determinar o grau de união dos pássaros. Dado um pássaro A, sua direção tenderá para o centro de gravidade (a posição média) de todos os pássaros dentro do seu campo de visão. O quão acentuada será esta tendência dependerá deste parâmetro de centralidade.
  - Avoid Factor \[F5 - 5\] : É o parâmetro para determinar a intensidade da repulsão entre dois pássaros que estão a uma distância mínima.
  - Match Coefficient \[F6 - 6\] : Este parâmetro de similaridade determina o grau com que cada pássaro tentará se adequar aos pássaros vizinhos, assumindo a direção de movimento do grupo. É o parâmetro que regula a contribuição, para a direção final de cada pássaro, do vetor médio das velocidades dos pássaros vizinhos. 
  - Correction Factor \[F7 - 7\] : Este parâmetro determina a intensidade com que os boids evitarão os extremos do limite do espaço (determinados artificialmente, sendo um cubo de 3 de lado) e os obstáculos da cena.

## Dificuldades em particular

A aplicação ainda não suporta um grande número de boids. Para conter um volume muito grande de boids, existem algumas estruturas de dados para acelerar o cálculo da direção de cada boid em função dos seus vizinhos mais próximos, e também seria possível realizar esse processamento pela GPU, entretanto ambas as soluções demandariam algum tempo.

É necessário estipular um limite de rotação da câmera pelo mouse pois ocorrem alguns pequenos glitches quando a câmera é alinhada com o vetor "up"/Y.

Existe um limite no campo de visão da câmera, então quando a câmera se afasta demais, os boids mais distantes desaparecem abruptamente. Gostaria de que esse afastamento fosse amenizado, deixando a imagem mais ofuscada, ou o boid cada vez menor conforme ele se afasta, ou então retirando o limite de visão. 

O modo como os boids evitam os obstáculos ainda não está ideal. O boid decide se mover quando a posição (central) do obstáculo está no escopo do seu campo de visão, mas isso ainda permite interseções dos boids com os obstáculos caso a escala do obstáculo seja muito grande. Idealmente, deveria ser possível calcular a distância entre a superfície do objeto e o boid.

## Referências

A aplicação é baseada no modelo clássico e mais básico proposto por Craig W. Raynolds. Referências em pseudocódigo podem ser encontradas em \[1\] e a página do criador do modelo está em \[2\].

- \[1\]: http://www.kfish.org/boids/pseudocode.html
- \[2\]: http://www.red3d.com/cwr/boids/

