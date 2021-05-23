file(GLOB_RECURSE CPPS  ./*.cpp )


SET(CMAKE_AUTOUIC ON)
#SET(SOURCES NodeForm.cpp)
#qt_wrap_cpp(SOURCES NodeForm.h)

add_executable(game ${CPPS} NodeForm.ui InfoForm.ui)
target_link_libraries(game nodes)
