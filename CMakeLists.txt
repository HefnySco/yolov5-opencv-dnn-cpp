cmake_minimum_required(VERSION 3.1.0)
project( YOLO5 )

if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

SET (CMAKE_C_COMPILER /usr/bin/gcc)
SET (CMAKE_CXX_COMPILER /usr/bin/g++)


# Local header files here ONLY
SET(TARGET_H
    yolo.hpp
    yolo_video_detector.hpp
    queue_fps.hpp   
   )

# Local source files here
SET(TARGET_SRC
    video_detector.cpp
    yolo.cpp
    yolo_video_detector.cpp
    queue_fps.cpp
    )



set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(OpenCV REQUIRED )
find_package(Threads REQUIRED)

#define default build
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
SET(CMAKE_BUILD_TYPE DEBUG)
ENDIF()


message ("${Yellow}=========================================================================${ColourReset}")
message ("CMAKE_C_COMPILER: "  ${CMAKE_C_COMPILER})
message ("CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER})

#deceide debug or release
IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
message("CMAKE_BUILD_TYPE: ${BoldYellow} DEBUG ${ColourReset}")
ELSEIF (CMAKE_BUILD_TYPE MATCHES RELEASE)
message("CMAKE_BUILD_TYPE: ${BoldGreen} RELEASE ${ColourReset}")
ENDIF()

message("OpenCV_VERSION : ${BoldGreen} ${OpenCV_VERSION} ${ColourReset}")

# OPTIONS
option(YOLO_USE_CUDA "Use CUDA if available." ON)
option(COMPILE_IMAGE "compile detect image." ON)
option(COMPILE_VIDEO "compile detect image." ON)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-D_GLIBCXX_DEBUG")
#set(CMAKE_CXX_FLAGS_RELEASE_INIT "-DRELEASE")
set(CMAKE_CXX_FLAGS "-lpthread")

IF(YOLO_USE_CUDA)
message("YOLO_USE_CUDA mode${BoldGreen} ON ${ColourReset}")
add_compile_definitions(
    YOLO_USE_CUDA
)
ELSE()
message("YOLO_USE_CUDA mode${BoldRed} OFF ${ColourReset}")
ENDIF (YOLO_USE_CUDA)

# adding CMAKE_CXX_FLAGS properly
add_compile_options(
  -Wno-return-type 
  -Wno-address-of-packed-member
  $<$<CONFIG:DEBUG>:-g3>
  $<$<CONFIG:DEBUG>:-Og>
  $<$<CONFIG:RELEASE>:-O3>
)

# adding DEFINEs
add_compile_definitions(
    $<$<CONFIG:DEBUG>:DEBUG>
    $<$<CONFIG:RELEASE>:RELEASE>
)

IF(COMPILE_IMAGE)
message("COMPILE_IMAGE mode ${BoldGreen} ON ${ColourReset}")
add_executable( yolo5_detector image_detector.cpp yolo.cpp)
target_link_libraries(yolo5_detector  ${OpenCV_LIBS} )
ELSE()
message("COMPILE_IMAGE mode ${BoldRED} OFF ${ColourReset}")
ENDIF()

IF(COMPILE_VIDEO)
message("COMPILE_VIDEO mode ${BoldGreen} ON ${ColourReset}")
add_executable( yolo5_video_detector ${TARGET_SRC} )
target_link_libraries(yolo5_video_detector   Threads::Threads ${OpenCV_LIBS} )
ELSE()
message("COMPILE_VIDEO mode ${BoldRED} OFF ${ColourReset}")
ENDIF()



message ("${Yellow}=========================================================================${ColourReset}")
