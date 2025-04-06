cmake_minimum_required(VERSION 3.10)
project(smart_home_rtos VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output binary to bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Check if sources and includes directories exist, if not create minimal structure
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src)
    message(STATUS "Source directory not found. Using current directory for sources.")
    set(SOURCES
        main.cpp
        # Add any other source files you have in the current directory
    )
else()
    set(SOURCES
        src/main.cpp
        src/Logger.cpp
        src/TaskManager.cpp
        src/Sensor.cpp
        src/TemperatureSensorTask.cpp
        src/WindowBlindController.cpp
        src/WindowBlindTask.cpp
        src/LightController.cpp
        src/LightControlTask.cpp
    )
endif()

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/include)
    message(STATUS "Include directory not found. Using current directory for headers.")
    set(HEADERS
        Logger.hpp
        TaskManager.hpp
        Sensor.hpp
        TemperatureSensorTask.hpp
        WindowBlindController.hpp
        WindowBlindTask.hpp
        LightController.hpp
        LightControlTask.hpp
    )
    
    # No separate include directory, use current directory
    set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
else()
    set(HEADERS
        include/Logger.hpp
        include/TaskManager.hpp
        include/Sensor.hpp
        include/TemperatureSensorTask.hpp
        include/WindowBlindController.hpp
        include/WindowBlindTask.hpp
        include/LightController.hpp
        include/LightControlTask.hpp
    )
    
    # Use the include directory
    set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()

# Create executable
add_executable(smart_home_rtos ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(smart_home_rtos PRIVATE ${INCLUDE_DIR})

# Add platform-specific threading library
find_package(Threads REQUIRED)
target_link_libraries(smart_home_rtos PRIVATE Threads::Threads)

# Set warning levels (good practice)
if(MSVC)
    target_compile_options(smart_home_rtos PRIVATE /W4)
else()
    target_compile_options(smart_home_rtos PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install target
install(TARGETS smart_home_rtos DESTINATION bin)

# Enable testing
enable_testing()

# Add option to build tests
option(BUILD_TESTS "Build the tests" OFF)
if(BUILD_TESTS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    add_subdirectory(tests)
endif()

# Add documentation generation with Doxygen if available
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif()