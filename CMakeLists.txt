# Minimum version of CMake (3.16+ is recommended for modern Qt CMake)
cmake_minimum_required(VERSION 3.16)

# Set the project name
project(FFT_Analyzer_App VERSION 1.0
    LANGUAGES CXX
)

# Enforce a modern C++ standard (Qt 6 requires C++17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable automatic MOC, UIC, and RCC
# MOC: Meta-Object Compiler (for Qt Signals/Slots)
# UIC: User Interface Compiler (for .ui files)
# RCC: Resource Compiler (for .qrc files)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find the required Qt components/modules
# Core and Gui are implicitly required by Widgets
find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Charts # Required for plotting your FFT data
)

# Define the source files for your executable
# List your main, header/source files, and the .ui file from Qt Designer.
# CMake's AUTOUIC will automatically process the .ui file.
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    audioprocessor.cpp 
    audioprocessor.h 
    # Add other C++ files here (e.g., fft_processor.cpp)
)

# Define and create the executable target
qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# Link the executable against the required Qt modules
# We link to the targets found by find_package()
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Widgets
        Qt6::Charts
)

# Optional: Set a clean output directory for the executable
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)