# ---[ Projects.
add_subdirectory ("aizcv")
add_subdirectory ("motion_heatmap_tool")

set_property (DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "motion_heatmap_tool")
