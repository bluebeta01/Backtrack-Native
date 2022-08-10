file(GLOB_RECURSE sources *.h *.c)
set(target_name zlib)
add_library(${target_name} ${sources})
set_target_properties(${target_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY
	${PROJECT_SOURCE_DIR}/bin/${target_name})