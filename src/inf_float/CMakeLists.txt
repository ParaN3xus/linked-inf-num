aux_source_directory(. SRCS)
add_library (inf_float ${SRCS})

target_link_libraries(inf_int utils)
