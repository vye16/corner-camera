include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/src)


add_executable( test_calibrate test_calibrate.cpp )
target_link_libraries( test_calibrate Infer ${OpenCV_LIBS} )

add_executable( test_config test_config.cpp )
target_link_libraries( test_config Infer ${OpenCV_LIBS} )

add_executable( test_video_infer test_video_infer.cpp )
target_link_libraries( test_video_infer Infer ${OpenCV_LIBS} )

add_executable( test_infer_funcs test_infer_funcs.cpp )
target_link_libraries( test_infer_funcs Infer ${OpenCV_LIBS} )

add_executable( test_webcam_infer test_webcam_infer.cpp )
target_link_libraries( test_webcam_infer Infer ${OpenCV_LIBS} )

#[[
add_executable( test_pointgrey test_pointgrey.cpp )
target_link_libraries( test_pointgrey Infer ${OpenCV_LIBS} ${FLYCAPTURE} )

add_executable( test_pointgrey_infer test_pointgrey_infer.cpp )
target_link_libraries( test_pointgrey_infer Infer ${OpenCV_LIBS} ${FLYCAPTURE} )
]]

if (FLYCAPTURE_FOUND)
  include_directories(${FLYCAPTURE_INCLUDE_DIRS})
  target_link_libraries (test_pointgrey ${FLYCAPTURE_LIBRARIES})
  target_link_libraries (test_pointgrey_infer ${FLYCAPTURE_LIBRARIES})
endif (FLYCAPTURE_FOUND)
