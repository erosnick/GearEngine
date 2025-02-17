﻿add_executable (GearEditor
        Source/main.cpp
        Source/CameraController.cpp
        Source/TextureImporter.cpp
        Source/GltfImporter.cpp
        Source/TestScene/AnimationTestScene.cpp
        Source/TestScene/ShadowTestScene.cpp
        Source/TestScene/MaterialTestScene.cpp)

target_link_libraries(GearEditor PRIVATE GearEngine)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/External/glfw EXCLUDE_FROM_ALL glfw.out)
target_link_libraries(GearEditor PRIVATE glfw)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/External/stb)
target_link_libraries(GearEditor PRIVATE stb)

# cgltf
add_library(cgltf INTERFACE)
target_include_directories(cgltf INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/External/cgltf)
target_link_libraries(GearEditor PRIVATE cgltf)

# imgui
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/External/imgui EXCLUDE_FROM_ALL imgui.out)
target_include_directories(GearEditor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/External/imgui)
target_link_libraries(GearEditor PRIVATE imgui)

# builtin resources
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/BuiltinResources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})