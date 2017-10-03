include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(Infer
	CalibrationWindow.cpp
	ConfigParser.cpp
	RollingDisplay.cpp
	Inference.cpp
	VideoInference.cpp
	WebCamInference.cpp
	#PointGreyInference.cpp
)
