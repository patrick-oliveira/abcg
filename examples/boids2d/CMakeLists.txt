project(boids2d)
add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp obstacles.cpp predator.cpp boids.cpp)
enable_abcg(${PROJECT_NAME})