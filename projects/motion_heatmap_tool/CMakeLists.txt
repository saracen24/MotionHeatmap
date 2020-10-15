# ---[ Project.
project (motion_heatmap
  VERSION 0.1.0.0
  DESCRIPTION "Motion heatmap library customization tool."
  LANGUAGES CXX
)

# ---[ Modules.
include (FetchContent)
include (GNUInstallDirs)

# ---[ Target.
add_executable (${PROJECT_NAME})

# ---[ Compile definitions.
string (TIMESTAMP BUILD_TIMESTAMP)
target_compile_definitions (${PROJECT_NAME}
  PRIVATE
    PROJECT_NAME="${PROJECT_NAME}"
    PROJECT_DESCRIPTION="${PROJECT_DESCRIPTION}"
    PROJECT_VERSION="${PROJECT_VERSION}"
    BUILD_TIMESTAMP="${BUILD_TIMESTAMP}"
)

# ---[ Dependencies.
# OpenCV
find_package (OpenCV 4 REQUIRED
  COMPONENTS
    highgui
)

# ---[ Include directories.
target_include_directories (${PROJECT_NAME}
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${OpenCV_INCLUDE_DIRS}"
)

# ---[ Link libraries.
target_link_libraries (${PROJECT_NAME}
  PRIVATE
    CONAN_PKG::cxxopts
    CONAN_PKG::nlohmann_json
    CONAN_PKG::fmt
    ${OpenCV_LIBS}
    $<$<PLATFORM_ID:Linux>:stdc++fs>
    aizcv::aizcv
)

# ---[ Compile options.
target_compile_options (${PROJECT_NAME}
  PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
      -Wall
      -Wextra
      -Wpedantic
      -pedantic-errors
    >
    $<$<CXX_COMPILER_ID:MSVC>:
      /Wall
      /EHsc
      /MP
    >
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:
      /Zi
      /MDd
    >
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:
      /MD
    >
)

# ---[ Sources.
set (SOURCE_FILES
  "./cfg/motion_heatmap_tool.json"
  "./src/main.cpp"
  "./src/processor.cpp"
  "./src/processor.hpp"
  "./src/utilities/cfg.cpp"
  "./src/utilities/cfg.hpp"
  "./src/utilities/cli.cpp"
  "./src/utilities/cli.hpp"
)

source_group (TREE "${CMAKE_CURRENT_SOURCE_DIR}"
  FILES
    ${SOURCE_FILES}
)

target_sources (${PROJECT_NAME}
  PRIVATE
    ${SOURCE_FILES}
)

# ---[ Target properties.
set_target_properties (${PROJECT_NAME}
  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    LINKER_LANGUAGE CXX
    LINK_WHAT_YOU_USE ON
    BUILD_RPATH_USE_ORIGIN ON
    BUILD_WITH_INSTALL_RPATH ON
    INSTALL_RPATH_USE_LINK_PATH ON
    BUILD_RPATH "${CMAKE_INSTALL_LIBDIR}"
    INSTALL_RPATH "${CMAKE_INSTALL_LIBDIR}"
    POSITION_INDEPENDENT_CODE ON
    DEBUG_POSTFIX "d"
    USE_FOLDERS ON
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}"
)

# ---[ Install rules.
include (InstallRequiredSystemLibraries)
install (TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
