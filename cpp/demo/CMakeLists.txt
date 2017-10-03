include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/src)


add_executable( run_video_infer run_video_infer.cpp )
target_link_libraries( run_video_infer Infer ${OpenCV_LIBS} )

add_executable( run_webcam_infer run_webcam_infer.cpp )
target_link_libraries( run_webcam_infer Infer ${OpenCV_LIBS} )

#[[
add_executable( run_pointgrey_infer run_pointgrey_infer.cpp )
target_link_libraries( run_pointgrey_infer Infer ${OpenCV_LIBS} ${FLYCAPTURE} )
]]

if (FLYCAPTURE_FOUND)
  include_directories(${FLYCAPTURE_INCLUDE_DIRS})
  target_link_libraries (run_pointgrey_infer ${FLYCAPTURE_LIBRARIES})
endif (FLYCAPTURE_FOUND)
