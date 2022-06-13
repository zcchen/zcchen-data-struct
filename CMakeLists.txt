cmake_minimum_required(VERSION 3.5)

PROJECT(ZcchenDataStruct
    VERSION 0.1.0
    LANGUAGES C CXX
)

# ---------------- Cross System Compiler -------------------
if (MSVC OR MINGW)
    SET(CMAKE_SYSTEM_NAME "Windows")
endif()
message(">>> CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message(">>> CMAKE_HOST_SYSTEM: ${CMAKE_HOST_SYSTEM}")
# ------------- End of Cross System Compiler ---------------

# ------------------ Compiler Standards ---------------------
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# --------------- End of Compiler Standars -----------------

# ------------------- Compiler Settings --------------------
if (MSVC)
elseif (APPLE)
else()
    add_definitions(-D_POSIX_C_SOURCE=200112L)
    add_definitions(-D_DEFAULT_SOURCE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
endif()
include(cmake/compiler_flags.cmake)
# --------------- End of Compiler Settings -----------------

# ------------------ Path settings ---------------------
#LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
if (CMAKE_SYSTEM_NAME MATCHES Windows)
elseif (CMAKE_SYSTEM_NAME MATCHES Darwin)
else()
endif()
# ------------------------------------------------------

ADD_SUBDIRECTORY(headers)
ADD_SUBDIRECTORY(descriptor)

