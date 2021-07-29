#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "globaldata.hpp"
#include "boids.hpp"
#include "obstacles.hpp"
#include "predator.hpp"


class OpenGLWindow : public abcg::OpenGLWindow { 
    protected:
        void handleEvent(SDL_Event& event) override;
        void initializeGL() override; 
        void paintGL() override;
        void paintUI() override;
        void resizeGL(int width, int height) override; 
        void terminateGL() override;

    private:
        GLuint m_boidsProgram{};
        GLuint m_obstaclesProgram{};

        int m_viewportWidth{};  
        int m_viewportHeight{};

        Boids m_boids;
        Obstacles m_obstacles;
        Predator m_predator;

        GlobalData m_globalData;

        std::default_random_engine m_randomEngine; 

        void update();
};

#endif