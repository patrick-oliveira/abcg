#include "openglwindow.hpp"
#include <imgui.h>
#include "abcg.hpp"
#include <string>
#include <iostream>

void OpenGLWindow::handleEvent(SDL_Event &event) {
  // O botao esquerdo do mouse determina se o predator esta ou nao incluso no modelo
  if(event.type == SDL_MOUSEBUTTONDOWN) {
    if(event.button.button == SDL_BUTTON_LEFT)
      m_globalData.m_removePredator = false;
  }
  if(event.type == SDL_MOUSEBUTTONUP) {
    if(event.button.button == SDL_BUTTON_LEFT)
      m_globalData.m_removePredator = true;
  }
}

void OpenGLWindow::initializeGL() {
  m_boidsProgram = createProgramFromFile(getAssetsPath() + "boids.vert", getAssetsPath() + "boids.frag"); // Estes arquivos sao utilizados tanto para os boids quanto para o predador
  m_obstaclesProgram = createProgramFromFile(getAssetsPath() + "obstacles.vert", getAssetsPath() + "obstacles.frag");

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

#if !defined(__EMSCRIPTEN__)
  glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  m_obstacles.initializeGL(m_obstaclesProgram, 3);
  m_boids.initializeGL(m_boidsProgram, 800, m_globalData, &m_obstacles);
  m_predator.initializeGL(m_boidsProgram);
}

void OpenGLWindow::update() {
  // Os obstaculos sao estaticos
  float deltaTime{static_cast<float>(getDeltaTime())};
  
  m_predator.update(m_globalData, &m_obstacles, deltaTime);
  m_boids.update(m_globalData, m_predator.m_translation, &m_obstacles, deltaTime);
}

void OpenGLWindow::paintGL() {
  update();

  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  m_obstacles.paintGL();
  if (m_globalData.m_removePredator == false) m_predator.paintGL(); // O predador nao eh pintado na tela se ele estiver desativado
                                                                    // As funcoes internas dos boids tambem confere se o predador esta ativado
  m_boids.paintGL();
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();
  {
    ImGui::SetNextWindowPos(ImVec2(5, m_viewportHeight - 270 - 5));
    ImGui::SetNextWindowSize(ImVec2(400, 270));
    auto flags{ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushItemWidth(100);
    // Parametros
    ImGui::SliderFloat("Visual Range", &m_boids.m_visualRange, 0.0f, 1.0f, "%.3f");
    ImGui::SliderFloat("Speed Limit", &m_boids.m_speedLimit, 0.0f, 3.0f, "%.3f");
    ImGui::SliderFloat("Min Distance", &m_boids.m_minDistance, 0.0f, 0.3f, "%.3f");
    ImGui::SliderFloat("Min Pred. Distance", &m_boids.m_minPredatorDistance, 0.0f, 0.3f, "%.3f");
    ImGui::SliderFloat("Centering Factor", &m_boids.m_centeringFactor, 0.0f, 1.0f, "%.3f");
    ImGui::SliderFloat("Avoid Factor", &m_boids.m_avoidFactor, 0.0f, 1.0f, "%.3f");
    ImGui::SliderFloat("Predator Avoid Factor", &m_boids.m_predatorAvoidFactor, 0.0f, 3.0f, "%.3f");
    ImGui::SliderFloat("Match Coefficient", &m_boids.m_matchingFactor, 0.0f, 0.3f, "%.3f");
    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportHeight = height;
  m_viewportWidth  = width;
  // Essa copia eh salva para pegar o vetor posicao nao-normalizado dos itens na tela
  m_globalData.viewportHeight = height;
  m_globalData.viewportWidth  = width;

  glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  glDeleteProgram(m_boidsProgram);
  glDeleteProgram(m_obstaclesProgram);
  m_obstacles.terminateGL();
  m_predator.terminateGL();
  m_boids.terminateGL();
}

