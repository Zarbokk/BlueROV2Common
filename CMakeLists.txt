cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CATKIN_ENABLE_TESTING OFF)
project(bluerov2common)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
#find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs roscpp message_generation cv_bridge ping360_sonar_msgs commonbluerovmsg dynamic_reconfigure waterlinked_dvl)
#find_package(PCL 1.10 REQUIRED COMPONENTS common io visualization)
#find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(eigen3_cmake_module REQUIRED)
find_package(Eigen3)
find_package(tf2 REQUIRED)

find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)
#find_package(Qt5 COMPONENTS Core Gui  Widgets Gamepad REQUIRED)
##find_package(OpenCV REQUIRED)
#find_package(Qt5PrintSupport REQUIRED)

find_package(rclcpp REQUIRED)
find_package(rclpy REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(commonbluerovmsg REQUIRED)
find_package(px4_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(waterlinked_a50 REQUIRED)
find_package(ping360_sonar_msgs REQUIRED)
find_package(cv_bridge REQUIRED)


############## LIBRARYS ##############
#add_library(generalhelpfullib SHARED src/generalTools/generalHelpfulTools.cpp)
#target_include_directories(generalhelpfullib PUBLIC  src/generalTools)
#ament_target_dependencies(generalhelpfullib tf2 Eigen3)

include_directories(src/generalTools
        ${QWT_INCLUDE_DIR})

############## Executables ##############

add_executable(ekfNode src/stateEstimation/ekfDVLnode.cpp src/stateEstimation/pose.h src/stateEstimation/ekfDVL.cpp src/stateEstimation/ekfDVL.h src/generalTools/generalHelpfulTools.cpp )
ament_target_dependencies(ekfNode rclcpp std_msgs sensor_msgs commonbluerovmsg waterlinked_a50 tf2 Eigen3 px4_msgs)
#target_link_libraries(ekfNode )


#add_executable(testFile src/testFile.cpp src/stateEstimation/pose.h src/stateEstimation/ekfDVL.cpp src/stateEstimation/ekfDVL.h )
#ament_target_dependencies(testFile rclcpp std_msgs sensor_msgs commonbluerovmsg)
#target_link_libraries(testFile generalhelpfullib )


add_executable(controllerbluerov2 src/controller/controllerOfBluerov2.h src/controller/controllerOfBluerov2.cpp src/controller/controllerRosNode.cpp)
ament_target_dependencies(controllerbluerov2 rclcpp std_msgs sensor_msgs commonbluerovmsg tf2 Eigen3 px4_msgs visualization_msgs)

add_executable(visualizationPing360 src/visualizationSonarImage.cpp)
ament_target_dependencies(visualizationPing360 rclcpp std_msgs sensor_msgs commonbluerovmsg ping360_sonar_msgs cv_bridge)


#add_executable(guibluerov2 src/gui/guiBlueROV2.cpp src/gui/mainwindow.h src/gui/mainwindow.cpp src/gui/rosHandlerGui.cpp src/gui/rosHandlerGui.h src/gui/qcustomplot.h src/gui/qcustomplot.cpp)
#ament_target_dependencies(guibluerov2 rclcpp std_msgs sensor_msgs commonbluerovmsg tf2 Eigen3 cv_bridge waterlinked_a50)
#target_link_libraries(guibluerov2 Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport Qt5::Gamepad)


install(TARGETS
        ekfNode
        controllerbluerov2
        visualizationPing360
#        guibluerov2
        DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch DESTINATION share/${PROJECT_NAME}/)


ament_python_install_package(${PROJECT_NAME})

install(PROGRAMS
        scripts/pwmServicesTopTube.py
        scripts/pwmServicesSensorTube.py
        scripts/leakageSensorTopTube.py
        scripts/leakageSensorSensorTube.py
        scripts/powerControlBotomTube.py
        scripts/mjpeg_cam_python.py
        bashScripts/xrcClientStart.sh
        bashScripts/startpigpiod.sh
        DESTINATION lib/${PROJECT_NAME}
)



ament_package()


