set(UTILITY_HEADERS
    JSONConvertable.hpp
    NotImplementedError.hpp
    NotSupportedError.hpp
    SimpleLogger.hpp
    TablePrinter.hpp
    VariadicTable.hpp
    JsonConverter.hpp
    )

    list(TRANSFORM UTILITY_HEADERS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
    set(UTILITY_HEADERS ${UTILITY_HEADERS} PARENT_SCOPE)