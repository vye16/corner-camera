include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/src)


add_executable( readFrame readFrame.cpp )
target_link_libraries( readFrame ${OpenCV_LIBS} )

add_executable( viewImage viewImage.cpp )
target_link_libraries( viewImage ${OpenCV_LIBS} )

add_executable( streamVideo streamVideo.cpp )
target_link_libraries( streamVideo ${OpenCV_LIBS} )

add_executable( writeVideo writeVideo.cpp )
target_link_libraries( writeVideo ${OpenCV_LIBS} )

add_executable( makeAmat makeAmat.cpp )
target_link_libraries( makeAmat ${OpenCV_LIBS} )

add_executable( runningMean runningMean.cpp )
target_link_libraries( runningMean ${OpenCV_LIBS} )

add_executable( selectRegion selectRegion.cpp )
target_link_libraries( selectRegion ${OpenCV_LIBS} )

add_executable( sampleRays sampleRays.cpp )
target_link_libraries( sampleRays ${OpenCV_LIBS} )

add_executable( multichanMul multichanMul.cpp )
target_link_libraries( multichanMul ${OpenCV_LIBS} )

add_executable( findFeatures findFeatures.cpp )
target_link_libraries( findFeatures ${OpenCV_LIBS} )

add_executable( matchFeatures matchFeatures.cpp )
target_link_libraries( matchFeatures ${OpenCV_LIBS} )

add_executable( houghlines houghlines.cpp )
target_link_libraries( houghlines ${OpenCV_LIBS} )

add_executable( lsdlines lsdlines.cpp )
target_link_libraries( lsdlines ${OpenCV_LIBS} )

add_executable( binarylines binarylines.cpp )
target_link_libraries( binarylines ${OpenCV_LIBS} )

if (DC1394_FOUND)
  add_executable( connectUSB connectUSB.cpp )
  target_link_libraries( connectUSB ${OpenCV_LIBS} ${DC1394} )
endif (DC1394_FOUND)
