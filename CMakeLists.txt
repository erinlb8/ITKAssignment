cmake_minimum_required(VERSION 2.8)

project (ITKAssignment)

find_package (ITK REQUIRED)
include (${ITK_USE_FILE})

add_executable (ITKAssignment ITKAssignment.cxx AffineRegistration.cxx DeformableRegistration.cxx)

target_link_libraries (ITKAssignment ${ITK_LIBRARIES})
