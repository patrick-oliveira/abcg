#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <tiny_obj_loader.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>

// Custom specialization of std::hash injected in namespace std
namespace std {
template <>
struct hash<Vertex> {
  size_t operator()(Vertex const& vertex) const noexcept {
    std::size_t h1{std::hash<glm::vec3>()(vertex.position)};
    return h1;
  }
};
}  // namespace std

void OpenGLWindow::handleEvent(SDL_Event& ev) {
  glm::ivec2 mousePosition;
  SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

  if (ev.type == SDL_MOUSEMOTION){
    m_camera.mouseMove(ev.motion.xrel, ev.motion.yrel);
  }


  if (ev.type == SDL_KEYDOWN) {
    if (ev.key.keysym.sym == SDLK_w) m_dollySpeed = m_camera.m_speed;
    if (ev.key.keysym.sym == SDLK_s) m_dollySpeed = -m_camera.m_speed;
    if (ev.key.keysym.sym == SDLK_a) m_truckSpeed = -m_camera.m_speed;
    if (ev.key.keysym.sym == SDLK_d) m_truckSpeed = m_camera.m_speed;
    if (ev.key.keysym.sym == SDLK_q) m_panSpeed = -m_camera.m_speed;
    if (ev.key.keysym.sym == SDLK_e) m_panSpeed = m_camera.m_speed;
    if (ev.key.keysym.sym == SDLK_i) m_verticalPanSpeed = m_camera.m_speed;
    if (ev.key.keysym.sym == SDLK_k) m_verticalPanSpeed = -m_camera.m_speed; 

    if (ev.key.keysym.sym == SDLK_LCTRL) ctrlPressed = true;

    if (ev.key.keysym.sym == SDLK_F1) m_boids.visualRange += 0.01;
    if (ev.key.keysym.sym == SDLK_F2) m_boids.speedFactor += 0.01;
    if (ev.key.keysym.sym == SDLK_F3) m_boids.minDistance += 0.01;
    if (ev.key.keysym.sym == SDLK_F4) m_boids.centeringFactor += 0.01;
    if (ev.key.keysym.sym == SDLK_F5) m_boids.avoidanceFactor += 0.01;
    if (ev.key.keysym.sym == SDLK_F6) m_boids.matchingFactor += 0.01;
    if (ev.key.keysym.sym == SDLK_F7) m_boids.correctionFactor += 0.01;

    
    if (ev.key.keysym.sym == SDLK_1) m_boids.visualRange -= 0.01;
    if (ev.key.keysym.sym == SDLK_2) m_boids.speedFactor -= 0.01;
    if (ev.key.keysym.sym == SDLK_3) m_boids.minDistance -= 0.01;
    if (ev.key.keysym.sym == SDLK_4) m_boids.centeringFactor -= 0.01;
    if (ev.key.keysym.sym == SDLK_5) m_boids.avoidanceFactor -= 0.01;
    if (ev.key.keysym.sym == SDLK_6) m_boids.matchingFactor -= 0.01;
    if (ev.key.keysym.sym == SDLK_7) m_boids.correctionFactor -= 0.01;
  }
  if (ev.type == SDL_KEYUP) {
    if (ev.key.keysym.sym == SDLK_w && m_dollySpeed > 0) m_dollySpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_s && m_dollySpeed < 0) m_dollySpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_a && m_truckSpeed < 0) m_truckSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_d && m_truckSpeed > 0) m_truckSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_q && m_panSpeed < 0) m_panSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_e && m_panSpeed > 0) m_panSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_i && m_verticalPanSpeed > 0) m_verticalPanSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_k && m_verticalPanSpeed < 0) m_verticalPanSpeed = 0.0f;

    if (ev.key.keysym.sym == SDLK_LCTRL) ctrlPressed = false;
  }

  if (ev.type == SDL_MOUSEBUTTONDOWN) {
    if(ev.button.button == SDL_BUTTON_LEFT){
      if (m_update == false)
        m_update = true;
      else
        m_update = false;
    }
  }

}

void OpenGLWindow::initializeGL() {
  SDL_SetRelativeMouseMode(SDL_TRUE);

  glClearColor(0.1, 0.1, 0.1, 1);

  // Enable depth buffering
  glEnable(GL_DEPTH_TEST);

  // Initialize Boids
  boid_program = createProgramFromFile(getAssetsPath() + "blinnphong.vert",
                                       getAssetsPath() + "blinnphong.frag");
  m_boidModel.loadFromFile(getAssetsPath() + "boid.obj");
  m_boidModel.setupVAO(boid_program);
  m_trianglesToDraw_boid = m_boidModel.getNumTriangles();
  m_boids.createBoids();

  // Initialize obstacles
  obstacle_program = createProgramFromFile(getAssetsPath() + "texture.vert",
                                           getAssetsPath() + "texture.frag");
  m_obstacleModel.loadDiffuseTexture(getAssetsPath() + "stone.jpg");
  m_obstacleModel.loadFromFile(getAssetsPath() + "obstacle.obj");
  m_obstacleModel.setupVAO(obstacle_program);
  m_trianglesToDraw_obstacle = m_obstacleModel.getNumTriangles();
  m_obstacle_Ka = m_obstacleModel.getKa();
  m_obstacle_Kd = m_obstacleModel.getKd();
  m_obstacle_Ks = m_obstacleModel.getKs();
  m_obstacle_shininess = m_obstacleModel.getShininess();
  m_obstacles.setupObstacles();


  light_program = createProgramFromFile(getAssetsPath() + "texture.vert",
                                              getAssetsPath() + "texture.frag");
  m_lightSourceModel.loadDiffuseTexture(getAssetsPath() + "fire.jpg");
  m_lightSourceModel.loadFromFile(getAssetsPath() + "fireball.obj");
  m_lightSourceModel.setupVAO(light_program);
  m_trianglesToDraw_lightSource = m_lightSourceModel.getNumTriangles();
  m_light_Ka = m_lightSourceModel.getKa();
  m_light_Kd = m_lightSourceModel.getKd();
  m_light_Ks = m_lightSourceModel.getKs();
  m_light_shininess = m_lightSourceModel.getShininess();
  m_lightSource.setupLightSource();
  m_mappingMode = 1;  // cylindrical

}

void OpenGLWindow::paintGL() {
  update();

  // Clear color buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  glUseProgram(boid_program);

  GLint viewMatrixLoc{glGetUniformLocation(boid_program, "viewMatrix")};
  GLint projMatrixLoc{glGetUniformLocation(boid_program, "projMatrix")};
  GLint modelMatrixLoc{glGetUniformLocation(boid_program, "modelMatrix")};
  GLint normalMatrixLoc{glGetUniformLocation(boid_program, "normalMatrix")};
  GLint lightPosLoc{glGetUniformLocation(boid_program, "lightPosWorldSpace")};
  GLint shininessLoc{glGetUniformLocation(boid_program, "shininess")};
  GLint IaLoc{glGetUniformLocation(boid_program, "Ia")};
  GLint IdLoc{glGetUniformLocation(boid_program, "Id")};
  GLint IsLoc{glGetUniformLocation(boid_program, "Is")};
  GLint KaLoc{glGetUniformLocation(boid_program, "Ka")};
  GLint KdLoc{glGetUniformLocation(boid_program, "Kd")};
  GLint KsLoc{glGetUniformLocation(boid_program, "Ks")};

  glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.m_projMatrix[0][0]);

  glUniform4fv(lightPosLoc, 1, &m_lightSource.lightSourcePosition.x);
  glUniform1f(shininessLoc, m_boid_shininess);
  glUniform4fv(IaLoc, 1, &m_Ia.x);
  glUniform4fv(IdLoc, 1, &m_Id.x);
  glUniform4fv(IsLoc, 1, &m_Is.x);
  glUniform4fv(KaLoc, 1, &m_boid_Ka.x);
  glUniform4fv(KdLoc, 1, &m_boid_Kd.x);
  glUniform4fv(KsLoc, 1, &m_boid_Ks.x);
  
  for (const auto index : iter::range(m_boids.num_boids)) {
    auto &boid{m_boids.boid_list.at(index)};
    glm::mat4 modelMatrix{1.0f};

    modelMatrix = glm::translate(modelMatrix, boid.position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(m_boids.scale));
    modelMatrix = glm::rotate(modelMatrix, boid.angle, boid.rotationAxis);

    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);
    auto modelViewMatrix{glm::mat3(m_camera.m_viewMatrix * modelMatrix)};
    glm::mat3 normalMatrix(glm::inverseTranspose(modelViewMatrix));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);
    m_boidModel.render(m_trianglesToDraw_boid);
  }



  glUseProgram(obstacle_program);

  viewMatrixLoc = glGetUniformLocation(obstacle_program, "viewMatrix");
  projMatrixLoc = glGetUniformLocation(obstacle_program, "projMatrix");
  modelMatrixLoc = glGetUniformLocation(obstacle_program, "modelMatrix");
  normalMatrixLoc = glGetUniformLocation(obstacle_program, "normalMatrix");
  lightPosLoc = glGetUniformLocation(obstacle_program, "lightPosWorldSpace");
  shininessLoc = glGetUniformLocation(obstacle_program, "shininess");
  IaLoc = glGetUniformLocation(obstacle_program, "Ia");
  IdLoc = glGetUniformLocation(obstacle_program, "Id");
  IsLoc = glGetUniformLocation(obstacle_program, "Is");
  KaLoc = glGetUniformLocation(obstacle_program, "Ka");
  KdLoc = glGetUniformLocation(obstacle_program, "Kd");
  KsLoc = glGetUniformLocation(obstacle_program, "Ks");
  GLint diffuseTexLoc{glGetUniformLocation(obstacle_program, "diffuseTex")};
  GLint mappingModeLoc{glGetUniformLocation(obstacle_program, "mappingMode")};
  
  glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.m_projMatrix[0][0]);
  glUniform1i(diffuseTexLoc, 0);
  glUniform1i(mappingModeLoc, m_mappingMode);

  glUniform4fv(lightPosLoc, 1, &m_lightSource.lightSourcePosition.x);

  glUniform1f(shininessLoc, m_obstacle_shininess);
  glUniform4fv(IaLoc, 1, &m_Ia.x);
  glUniform4fv(IdLoc, 1, &m_Id.x);
  glUniform4fv(IsLoc, 1, &m_Is.x);
  glUniform4fv(KaLoc, 1, &m_obstacle_Ka.x);
  glUniform4fv(KdLoc, 1, &m_obstacle_Kd.x);
  glUniform4fv(KsLoc, 1, &m_obstacle_Ks.x);

  for (const auto index : iter::range(m_obstacles.num_obstacles)) {
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, m_obstacles.obstaclePositionList.at(index));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(m_obstacles.scale));

    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);
    auto modelViewMatrix{glm::mat3(m_camera.m_viewMatrix * modelMatrix)};
    glm::mat3 normalMatrix(glm::inverseTranspose(modelViewMatrix));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);
    m_obstacleModel.render(m_trianglesToDraw_obstacle);
  }



  glUseProgram(light_program);

  viewMatrixLoc = glGetUniformLocation(light_program, "viewMatrix");
  projMatrixLoc = glGetUniformLocation(light_program, "projMatrix");
  modelMatrixLoc = glGetUniformLocation(light_program, "modelMatrix");
  normalMatrixLoc = glGetUniformLocation(light_program, "normalMatrix");
  lightPosLoc = glGetUniformLocation(light_program, "lightPosWorldSpace");
  shininessLoc = glGetUniformLocation(light_program, "shininess");
  IaLoc = glGetUniformLocation(light_program, "Ia");
  IdLoc = glGetUniformLocation(light_program, "Id");
  IsLoc = glGetUniformLocation(light_program, "Is");
  KaLoc = glGetUniformLocation(light_program, "Ka");
  KdLoc = glGetUniformLocation(light_program, "Kd");
  KsLoc = glGetUniformLocation(light_program, "Ks");
  diffuseTexLoc = glGetUniformLocation(light_program, "diffuseTex");
  mappingModeLoc = glGetUniformLocation(light_program, "mappingMode");

  glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.m_projMatrix[0][0]);
  glUniform1i(diffuseTexLoc, 0);
  glUniform1i(mappingModeLoc, m_mappingMode);

  glm::vec4 top{glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)};
  glUniform4fv(lightPosLoc, 1, &top.x);

  glUniform1f(shininessLoc, m_light_shininess);
  glUniform4fv(IaLoc, 1, &m_Ia.x);
  glUniform4fv(IdLoc, 1, &m_Id.x);
  glUniform4fv(IsLoc, 1, &m_Is.x);
  glUniform4fv(KaLoc, 1, &m_light_Ka.x);
  glUniform4fv(KdLoc, 1, &m_light_Kd.x);
  glUniform4fv(KsLoc, 1, &m_light_Ks.x);

  glm::mat4 modelMatrix{1.0f};
  modelMatrix = glm::translate(modelMatrix, m_lightSource.lightSourcePosition);
  modelMatrix = glm::scale(modelMatrix, glm::vec3(m_lightSource.scale));

  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);
  auto modelViewMatrix{glm::mat3(m_camera.m_viewMatrix * modelMatrix)};
  glm::mat3 normalMatrix(glm::inverseTranspose(modelViewMatrix));
  glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);
  m_lightSourceModel.render(m_trianglesToDraw_lightSource);


  glUseProgram(0);
  
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();
  {
    ImGui::SetNextWindowPos(ImVec2(5, m_viewportHeight - 270 - 5));
    ImGui::SetNextWindowSize(ImVec2(400, 270));
    auto flags{ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::Text("visual range = %.2f", m_boids.visualRange);
    ImGui::Text("speed factor = %.2f", m_boids.speedFactor);
    ImGui::Text("min distance = %.2f", m_boids.minDistance);
    ImGui::Text("centering factor = %.2f", m_boids.centeringFactor);
    ImGui::Text("avoidance factor = %.2f", m_boids.avoidanceFactor);
    ImGui::Text("matching factor = %.2f", m_boids.matchingFactor);
    ImGui::Text("correction factor = %.2f", m_boids.correctionFactor);
    // ImGui::Text("m_lastMousePosition = (%f, %f)", m_camera.m_lastMousePosition.x, m_camera.m_lastMousePosition.y);
    // ImGui::Text("m_mouseMovementDirection = (%f, %f)", m_camera.m_mouseMovementDirection.x, m_camera.m_mouseMovementDirection.y);
    ImGui::End();
  }
  
  }

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() { 

  glDeleteProgram(boid_program);
  glDeleteProgram(obstacle_program); 
  glDeleteProgram(light_program);

}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};

  if (m_update == true){
    m_boids.update(deltaTime, &m_obstacles, &m_lightSource);
    m_obstacles.update(deltaTime);
    m_lightSource.update(deltaTime);
  }

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
  m_camera.verticalPan(m_verticalPanSpeed * deltaTime);
}