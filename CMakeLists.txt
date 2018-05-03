cmake_minimum_required(VERSION 2.8)
 
PROJECT(Iron_data)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(Iron_data MACOSX_BUNDLE Iron_data)
 
if(VTK_LIBRARIES)
  target_link_libraries(Iron_data ${VTK_LIBRARIES})
else()
  target_link_libraries(Iron_data vtkHybrid vtkWidgets)
endif()