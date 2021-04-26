#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include "abcg.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "boids.hpp"
#include "obstacles.hpp"
#include "lightsource.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint boid_program{};
  GLuint obstacle_program{};
  GLuint light_program{};
  
  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};
  int m_viewportWidth{};
  int m_viewportHeight{};

  Camera m_camera;
  float m_dollySpeed{0.0f};
  float m_truckSpeed{0.0f};
  float m_panSpeed{0.0f};
  float m_verticalPanSpeed{0.0f};

  Model m_boidModel;
  int m_trianglesToDraw_boid{};
  Boids m_boids;

  Model m_obstacleModel;
  int m_trianglesToDraw_obstacle{};
  Obstacles m_obstacles;

  bool ctrlPressed{false};
  bool m_update{true};
  int frame{0};

  // Light and material properties
  Model m_lightSourceModel;
  int m_trianglesToDraw_lightSource{};
  LightSource m_lightSource;

  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_obstacle_Ka;
  glm::vec4 m_obstacle_Kd;
  glm::vec4 m_obstacle_Ks;
  float m_obstacle_shininess{};
  glm::vec4 m_light_Ka;
  glm::vec4 m_light_Kd;
  glm::vec4 m_light_Ks;
  float m_light_shininess{};
  glm::vec4 m_boid_Ka{0.440f, 0.019f, 0.743f, 0.1f};
  glm::vec4 m_boid_Kd{0.333f, 0.283f, 0.283f, 1.0f};
  glm::vec4 m_boid_Ks{1.0f, 1.0f, 1.0f, 1.0f};
  float m_boid_shininess{30.0f};

  int m_mappingMode{};

  void update();
};

#endif