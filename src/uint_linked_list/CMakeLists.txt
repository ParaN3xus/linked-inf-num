aux_source_directory(. SRCS)
add_library (uint_linked_list ${SRCS})

target_link_libraries(uint_linked_list utils)
