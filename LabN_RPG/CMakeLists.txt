cmake_minimum_required(VERSION 3.15)

include_directories(
${CMAKE_CURRENT_LIST_DIR}/src/include/
${CMAKE_CURRENT_LIST_DIR}/src/include/Common/)

set(LABN_SOURCES
${CMAKE_CURRENT_LIST_DIR}/src/src/Main.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Game.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Common/GameplayUtils.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Common/AnimationStates/IdleAnimationState.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Common/AnimationStates/MovementAnimationState.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Common/SpacePartitioning/PartitionCell.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Common/SpacePartitioning/PartitionGrid.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Input/Keyboard.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Input/Mouse.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Prefabs/ActorPrefab.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Prefabs/MapFactory.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Systems/Gizmo/ColliderDebugRenderSystem.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Systems/Gizmo/PivotDebugRenderSystem.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Systems/Gizmo/SpatialPartitionDebugRenderSystem.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Systems/ActorMovementSystem.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Systems/AnimationStateSystem.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Systems/AnimationSystem.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Systems/ApplyTransformSystem.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Systems/CameraFollowingSystem.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Systems/CollisionSystem.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Systems/PlayerControllerSystem.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Systems/SpriteRenderSystem.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/Window/Window.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/World/World.cpp
${CMAKE_CURRENT_LIST_DIR}/src/src/World/GameWorld.cpp)

add_executable(LabN_RPG ${LABN_SOURCES})

target_compile_features(LabN_RPG PUBLIC cxx_std_20)

find_library(SFML_LIB 
NAMES sfml-audio sfml-graphics sfml-main sfml-network sfml-system sfml-window
HINTS ${CMAKE_BINARY_DIR}/SFML/)

target_include_directories(LabN_RPG 
PUBLIC ${PROJECT_SOURCE_DIR}/Entt/src/
PUBLIC ${PROJECT_SOURCE_DIR}/SFML/include/)

target_link_libraries(LabN_RPG
sfml-audio
sfml-graphics
sfml-main
sfml-network
sfml-system
sfml-window)

add_custom_command(TARGET LabN_RPG POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_LIST_DIR}/resources
    ${CMAKE_BINARY_DIR}/LabN_RPG/resources
    COMMENT "Copying resources")

add_custom_command(TARGET LabN_RPG POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_BINARY_DIR}/SFML/bin
    ${CMAKE_BINARY_DIR}/LabN_RPG
    COMMENT "Copying dlls")


