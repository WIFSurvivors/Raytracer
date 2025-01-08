set(UTILITY_HEADERS
    JSONConvertable.hpp
    NotImplementedError.hpp
    NotSupportedError.hpp
    Log.hpp
    TablePrinter.hpp
    VariadicTable.hpp
    JsonConverter.hpp
    bvhtree_tiny.hpp
	FrameSnapshot.hpp
    obj_loader.hpp
	BigJsonRedear.hpp
	quicktype_json.hpp
    Canvas.hpp
    data_loader.hpp
)

list(TRANSFORM UTILITY_HEADERS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(UTILITY_HEADERS ${UTILITY_HEADERS} PARENT_SCOPE)
