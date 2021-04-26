project(boids3d)
add_executable(${PROJECT_NAME} camera.cpp main.cpp openglwindow.cpp model.cpp boids.cpp obstacles.cpp lightsource.cpp)
enable_abcg(${PROJECT_NAME})
