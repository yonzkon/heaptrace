cmake_minimum_required(VERSION 2.8)

file(GLOB SRC *.c)
file(GLOB INC *.h)

add_library(heaptrace SHARED ${SRC})
target_link_libraries(heaptrace dl)
set_target_properties(heaptrace PROPERTIES VERSION 0.1.0 SOVERSION 0.1)

install(TARGETS heaptrace
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)

install(FILES ${INC} DESTINATION include/heaptrace)
