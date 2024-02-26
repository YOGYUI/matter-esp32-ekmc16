set(PRIV_INCLUDE_DIRS_LIST
     "${CMAKE_CURRENT_LIST_DIR}/include"
     "${CMAKE_CURRENT_LIST_DIR}/include/device"
     "${CMAKE_CURRENT_LIST_DIR}/include/peripheral"
     "${CMAKE_CURRENT_LIST_DIR}/include/system"
)

set(SRC_DIRS_LIST
     "${CMAKE_CURRENT_LIST_DIR}"
     "${CMAKE_CURRENT_LIST_DIR}/src/device"
     "${CMAKE_CURRENT_LIST_DIR}/src/peripheral"
     "${CMAKE_CURRENT_LIST_DIR}/src/system"
)

set(EXCLUDE_SRCS_LIST "")
set(PRIV_REQUIRES_LIST "")
list(APPEND PRIV_REQUIRES_LIST 
     device 
     esp_matter 
     esp_matter_bridge
     esp_matter_console 
     app_reset 
     json
)

idf_component_register(
     PRIV_INCLUDE_DIRS   ${PRIV_INCLUDE_DIRS_LIST}
     SRC_DIRS            ${SRC_DIRS_LIST}
     EXCLUDE_SRCS        ${EXCLUDE_SRCS_LIST}
     PRIV_REQUIRES       ${PRIV_REQUIRES_LIST}
)

set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 17)
target_compile_options(${COMPONENT_LIB} PRIVATE "-DCHIP_HAVE_CONFIG_H")
