cmake_minimum_required (VERSION 3.15...3.18 FATAL_ERROR)

# ---[ Solution.
project (MotionHeatmap
  VERSION 0.1.0.0
  DESCRIPTION "Motion heatmap accumulator."
  LANGUAGES CXX
)

# ---[ Options.
option (BUILD_SHARED_LIBS "Build shared libraries." ON)
option (BUILD_PACKAGING "Build distribution packages using CPack." ON)

# ---[ Conan.
if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message (STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file (DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake"
    TIMEOUT 5
    STATUS CONAN_CMAKE_DOWNLOAD_RESULT
  )
  if (NOT CONAN_CMAKE_DOWNLOAD_RESULT EQUAL 0)
    message (WARNING "Failed conan.cmake downloading. Error: ${CONAN_CMAKE_DOWNLOAD_RESULT}. Local copy is used.")
    file (COPY "${CMAKE_SOURCE_DIR}/cmake/conan.cmake" DESTINATION "${CMAKE_BINARY_DIR}")
  endif ()
endif()
include ("${CMAKE_BINARY_DIR}/conan.cmake")

conan_check (VERSION 1.30.1 REQUIRED)
include (GNUInstallDirs)
set (CONAN_IMPORTS
  "bin, *.dll -> ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}"
  "lib, *.dylib -> ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}"
  "lib, *.so -> ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}"
  "lib, *.so.* -> ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}"
)
conan_cmake_run (
  REQUIRES
    "cxxopts/2.2.1"
    "nlohmann_json/3.9.1"
    "fmt/7.0.3"
  OPTIONS
    fmt:shared=True
  SETTINGS compiler.cppstd=17
  IMPORTS ${CONAN_IMPORTS}
  BUILD missing
  BASIC_SETUP CMAKE_TARGETS KEEP_RPATHS NO_OUTPUT_DIRS
)

# ---[ Projects.
add_subdirectory ("projects")

# ---[ Pack rules.
if (BUILD_PACKAGING)
  set (CPACK_PACKAGE_NAME "${PROJECT_NAME}")
  set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
  set (CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
  set (CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
  set (CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
  set (CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
  include (CPack)
endif ()
