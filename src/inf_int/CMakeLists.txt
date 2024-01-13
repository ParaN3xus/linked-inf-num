aux_source_directory(. SRCS)
add_library (inf_int ${SRCS})

target_link_libraries(inf_int uint_linked_list utils)
