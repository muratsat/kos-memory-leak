cmake_minimum_required(VERSION 3.12)

# Toolchain for working with the NK-parser.
include (platform/nk)

# Set compiler flags.
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -Wno-deprecated-declarations")

add_executable (Subscriber "src/main.cpp" "src/subscriber.cpp"  "src/gpio.cpp")
#add_executable (Subscriber "src/gpio.cpp")
add_dependencies (Subscriber subscriber_edl_files)
target_link_libraries(Subscriber 
                      ${mosquitto_CXX_LIB}
                      ${vfs_CLIENT_LIB}

)

target_link_libraries (Subscriber ${gpio_CLIENT_LIB})

if ("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "arm|aarch64")
    target_link_libraries (Subscriber ${bsp_CLIENT_LIB})
endif ()



# We do not need the default VFS entity which comes from ${vfs_CLIENT_LIB}.
set_target_properties (Subscriber PROPERTIES ${vfs_ENTITY}_REPLACEMENT "")
set_target_properties (Subscriber PROPERTIES LINK_FLAGS ${VULN_LDFLAGS})
