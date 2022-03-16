cmake_minimum_required(VERSION 2.8)
project( YOLO5 )
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package( OpenCV REQUIRED )

IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
message("debug mode")
ELSE (CMAKE_BUILD_TYPE MATCHES RELEASE)
message("release mode")
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)

option(YOLO_USE_CUDA "do not access any CUDA." ON)

#set(CMAKE_CXX_FLAGS_DEBUG_INIT "-DDEBUG")
#set(CMAKE_CXX_FLAGS_RELEASE_INIT "-DRELEASE")

IF(YOLO_USE_CUDA)
message("YOLO_USE_CUDA mode ON")
add_compile_definitions(
    YOLO_USE_CUDA
)
ENDIF (YOLO_USE_CUDA)


add_compile_options(
  -Wno-return-type 
  -Wno-address-of-packed-member
  $<$<CONFIG:DEBUG>:-g3>
  $<$<CONFIG:DEBUG>:-Og>
  $<$<CONFIG:RELEASE>:-O3>
)

add_compile_definitions(
    $<$<CONFIG:DEBUG>:DEBUG>
    $<$<CONFIG:RELEASE>:RELEASE>
)
add_executable( yolo5_detector main.cpp yolo.cpp)
target_link_libraries(yolo5_detector  ${OpenCV_LIBS} )