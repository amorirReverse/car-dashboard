# ==============================================================================
# @file CMakeLists.txt
# @brief Script de configuration CMake pour la compilation du projet CarDashboard.
# ==============================================================================

cmake_minimum_required(VERSION 3.16)
project(CarDashboard VERSION 1.0 LANGUAGE CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Activation des générateurs automatiques Qt (MOC est obligatoire pour Q_OBJECT/CanReceiver)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Qml)

add_executable(CarDashboard
    src/main.cpp
    src/CanReceiver.h
    src/CanReceiver.cpp
)

# Déclaration du module QML pour l'intégration de main.qml
qt_add_qml_module(CarDashboard
    URI CarDashboard
    VERSION 1.0
    QML_FILES
        src/main.qml
)

# Édition des liens avec les bibliothèques Qt6
target_link_libraries(CarDashboard PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
)