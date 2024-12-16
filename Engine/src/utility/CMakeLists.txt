set(UTILITY_SOURCES 
    NotImplementedError.cpp
    NotSupportedError.cpp
    SimpleLogger.cpp
    TablePrinter.cpp
)
list(TRANSFORM UTILITY_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(UTILITY_SOURCES ${UTILITY_SOURCES} PARENT_SCOPE)