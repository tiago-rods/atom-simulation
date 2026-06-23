cmake_minimum_required(VERSION 3.20)
project(atom-simulation VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# ── GLFW ──────────────────────────────────────────────────────────────────────
FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
    GIT_SHALLOW    TRUE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# ── GLAD (OpenGL 3.3 Core loader) ─────────────────────────────────────────────
FetchContent_Declare(glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        v2.0.8
    GIT_SHALLOW    TRUE
    SOURCE_SUBDIR  cmake)

# ── GLM ───────────────────────────────────────────────────────────────────────
FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
    GIT_SHALLOW    TRUE)

FetchContent_MakeAvailable(glfw glad glm)

glad_add_library(glad_gl REPRODUCIBLE API gl:core=3.3)

# ── Executable ────────────────────────────────────────────────────────────────
add_executable(atom-simulation
    src/main.cpp
    src/app/Application.cpp
    src/rendering/Shader.cpp
    src/rendering/Camera.cpp
    src/rendering/ParticleBuffer.cpp
    src/rendering/OrbitalRenderer.cpp
    src/core/math/SpecialFunctions.cpp
    src/core/math/SphericalCoords.cpp
    src/core/physics/HydrogenOrbital.cpp
    src/core/sampling/RejectionSampler.cpp
    src/core/sampling/CDFSampler.cpp
    src/simulation/OrbitalSimulation.cpp
    src/ui/OrbitalCommands.cpp
    src/factory/OrbitalFactory.cpp
)

target_link_libraries(atom-simulation PRIVATE glfw glad_gl glm::glm)
target_include_directories(atom-simulation PRIVATE src)
