# Simulador de Murmúrio de Pássaros (Flocking Behaviour, ou "Boids")
## Autoria
Patrick Oliveira de Paula - RA: 11002616

## Recursos do Programa

- É possível incluir um predator no modelo apertando o botão esquerdo do mouse, e ele permanecerá na tela enquanto o botão for segurado. O predador andará automaticamente na direção apontada pelo mouse, parando exatamente em cima do local apontado. Os outros indivíduos reagirão à presença do predador tentando se afastar dele em uma intensidade governada por um parâmetro "Predator Avoid Factor".
- O programa inclui automaticamente um número ajustável de obstáculos de tamanho variável. O número de obstáculos e seus formatos precisa ser ajustado diretamente no código. Não achei que seria relevante deixar este parâmetro ajustável em tempo real, em particular porque tanto os pássaros quanto o predator se ajustam automaticamente ao se aproximar de um obstáculo, rotacionando o seu vetor de direção para que fique tangente ao obstáculo, desviando-o portanto. Um volume muito grande de obstáculos dispersaria demais os pássaros.
- O modelo envolve 8 parâmetros ajustáveis por "Sliders":
  - Exemplo\[Limite_Inferior, Limite_Superior\]: Função
  - Visual Range\[0.0, 1.0\] : Determina o raio do círculo centrado em cada pássaro e que delimita o seu círculo de visão. Para um pássaro A, todo pássaro B que estiver dentro do círculo de visão será incluído nos cálculos de atualização da direção de A.
  - Speed Limit\[0.0, 3.0\]  : Determina a velocidade máxima (em módulo) dos pássaros. A velocidade contribui para determinar a coerência dos pássaros pois quanto maior a velocidade, mais facilmente cada pássaro escapa dos grupos que se formam espontaneamente.
  - Min Distance\[0.0, 0.3\] : Determina a menor distância que pode haver entre qualquer par de pássaros. Quando dois pássaros A e B estão distanciados por este limiar (ou menos), sua velocidade é incrementada nas direções mutuamente opostas, a fim de que se afastem, com intensidade a depender do parâmetro "Avoid Factor".
  - Min Pred. Distance\[0.0, 0.3\] : Tem função análoga a do parâmetro anterior, porém agora a distância é em relação a cada pássaro e o predador.
  - Centering Factor \[0.0, 1.0\]  : É o parâmetro utilizado para determinar o grau de união dos pássaros. Dado um pássaro A, sua direção tenderá para o centro de gravidade (a posição média) de todos os pássaros dentro do seu campo de visão. O quão acentuada será esta tendência dependerá deste parâmetro de centralidade.
  - Avoid Factor \[0.0, 1.0\] : É o parâmetro para determinar a intensidade da repulsão entre dois pássaros que estão a uma distância mínima.
  - Predator Avoid Factor \[0.0, 3.0\] : Tem função análoga a do parâmetro anterior, mas agora em relação ao predator.
  - Match Coefficient \[0.0, 0.3\] : Este parâmetro de similaridade determina o grau com que cada pássaro tentará se adequar aos pássaros vizinhos, assumindo a direção de movimento do grupo. É o parâmetro que regula a contribuição, para a direção final de cada pássaro, do vetor médio das velocidades dos pássaros vizinhos. 

Estes parâmetros podem ser ajustados para observar o comportamento global dos pássaros. Por exemplo, um ângulo de visual menor contribuirá para que os pássaros se organizem em grupos pequenos, um parâmetro de similaridade ou de centralidade altos contribuirão para que os grupos sejam coerentes e não se quebrem tão facilmente, e de modo geral os pássaros terminarão formando um único grupo (que pode ser perturbado pela presença do predador). Uma velocidade alta contribui para a desagregação dos grupos, e que podem se agregar novamente com maior ou menor facilidade a depender dos outros parâmetros. Um parâmetro de repulsão elevado tenderá a fazer com que o grupo de espalhe pelo cenário, não necessário se desagregando (mas isso pode ser particularmente mais fácil quando a velocidade é maior).

Os pássaros também se ajustam automaticamente, por meio de um termo de correção adicionado às suas velocidades, à janela da aplicação, mantendo-se no campo de visão. Eles escapam para fora (e voltam novamente) com maior facilidade se a velocidade é mais alta, mas o termo de correção pode ser ajustado (variável "correction" em GlobalData) para que os pássaros fiquem estritamente dentro do escopo da janela.

## Dificuldades em particular

A aplicação suporta com facilidade 500 pássaros na tela. Imagino que meu computador consegue incluir mais elementos na tela, portanto isso dever devido à programação ineficiente. A aplicação precisa percorrer todos os pássaros a cada frame, ajustando os atributos de cada pássaro em função dos atributos de (potencialmente) todos os outros, incluindo obstáculos e o predator. Todo esse processo acaba sendo deveras custoso.

A solução para que o predador desvie dos obstáculos também não está ideal e pode ocorrer dele entrar na região de um obstáculo (mas não fica preso lá). A dificuldade esteve no fato de que o vetor velocidade do predador era constantemente atualizado pela posição do mouse, conflitando com o ajuste do desvio pela tangente do osbtáculo.

## Referências

A aplicação é baseada no modelo clássico e mais básico proposto por Craig W. Raynolds. Referências em pseudocódigo podem ser encontradas em \[1\] e a página do criador do modelo está em \[2\].

- \[1\]: http://www.kfish.org/boids/pseudocode.html
- \[2\]: http://www.red3d.com/cwr/boids/

