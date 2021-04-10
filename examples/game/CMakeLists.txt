file(GLOB_RECURSE CPPS  ./*.cpp )

add_executable(game ${CPPS})

target_link_libraries(game nodes)

