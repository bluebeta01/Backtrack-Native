file(GLOB_RECURSE sources *.h *.cpp)
set(target_name backtrack)
add_executable(${target_name} ${sources})
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
	${PROJECT_SOURCE_DIR}/bin/${target_name})
include_directories(src)
include_directories(${PROJECT_SOURCE_DIR}/btalib/src)

target_link_libraries(${target_name}
	btalib
	zlib
)