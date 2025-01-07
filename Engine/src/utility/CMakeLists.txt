set(UTILITY_SOURCES 
    NotImplementedError.cpp
    NotSupportedError.cpp
    Log.cpp
    TablePrinter.cpp
	FrameSnapshot.cpp
	BigJsonReader.cpp
)
list(TRANSFORM UTILITY_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(UTILITY_SOURCES ${UTILITY_SOURCES} PARENT_SCOPE)
