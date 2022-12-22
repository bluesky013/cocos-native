cmake_minimum_required(VERSION 3.6)

project(Cocos-Native)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})
get_filename_component(COCOS_NATIVE_PATH "${COCOS_ROOT_PATH}/native" ABSOLUTE)

set(CMAKE_CXX_STANDARD 17)
option(CC_DEBUG_FORCE           "Force enable CC_DEBUG in release mode" OFF)
option(USE_SE_V8                "Use V8 JavaScript Engine"              ON)
option(USE_V8_DEBUGGER          "Compile v8 inspector ws server"        OFF)
option(USE_V8_DEBUGGER_FORCE    "Force enable debugger in release mode" OFF)
option(USE_SOCKET               "Enable WebSocket & SocketIO"           OFF)
option(USE_AUDIO                "Enable Audio"                          OFF)     #Enable AudioEngine
option(USE_EDIT_BOX             "Enable EditBox"                        OFF)
option(USE_SE_JSC               "Use JavaScriptCore on MacOSX/iOS"      OFF)
option(USE_VIDEO                "Enable VideoPlayer Component"          OFF)
option(USE_WEBVIEW              "Enable WebView Component"              OFF)
option(USE_MIDDLEWARE           "Enable Middleware"                     ON)
option(USE_DRAGONBONES          "Enable Dragonbones"                    ON)
option(USE_SPINE                "Enable Spine"                          ON)
option(USE_WEBSOCKET_SERVER     "Enable WebSocket Server"               OFF)
option(USE_JOB_SYSTEM_TASKFLOW  "Use taskflow as job system backend"    OFF)
option(USE_JOB_SYSTEM_TBB       "Use tbb as job system backend"         OFF)
option(USE_PHYSICS_PHYSX        "Use PhysX Physics"                     ON)
option(USE_OCCLUSION_QUERY      "Use Occlusion Query"                   OFF)
option(USE_DEBUG_RENDERER       "Use Debug Renderer"                    OFF)
option(USE_GEOMETRY_RENDERER    "Use Geometry Renderer"                 OFF)
option(USE_WEBP                 "Use Webp"                              ON)

include(cmake/functions.cmake)
include(cmake/thirdparty.cmake)
include(cmake/configuration.cmake)

include(${COCOS_NATIVE_PATH}/CMakeLists.txt)

include(game/game_files.cmake)
include(core/core_files.cmake)
include(render/render_files.cmake)

configure_file(ProjectRoot.h.in ${CMAKE_BINARY_DIR}/Gen/ProjectRoot.h)
include_directories(${CMAKE_BINARY_DIR}/Gen)

set(NATIVE_APP cocos-native)

add_executable(${NATIVE_APP}
    main.cpp
    GameApp.cpp
    GameApp.h
    GameManager.h
    GameManager.cpp
    ${GAME_FILES}
    ${CORE_FILES}
    ${RENDER_FILES}
)

target_link_libraries(${NATIVE_APP}
    3rdParty::taskflow
    3rdParty::sfmt
    3rdParty::imgui
    3rdParty::entt
)

target_include_directories(${NATIVE_APP} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

if(MSVC)
    foreach(item ${WINDOWS_DLLS})
        get_filename_component(filename ${item} NAME)
        get_filename_component(abs ${item} ABSOLUTE)
        add_custom_command(TARGET ${NATIVE_APP} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${abs} $<TARGET_FILE_DIR:${NATIVE_APP}>/${filename}
                )
    endforeach()
    foreach(item ${V8_DLLS})
        get_filename_component(filename ${item} NAME)
        add_custom_command(TARGET ${NATIVE_APP} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${V8_DIR}/$<IF:$<BOOL:$<CONFIG:RELEASE>>,Release,Debug>/${filename} $<TARGET_FILE_DIR:${NATIVE_APP}>/${filename}
                )
    endforeach()
    target_link_options(${NATIVE_APP} PRIVATE /SUBSYSTEM:WINDOWS)
endif()
target_link_libraries(${NATIVE_APP} ${ENGINE_NAME})
target_include_directories(${NATIVE_APP} PRIVATE game)

