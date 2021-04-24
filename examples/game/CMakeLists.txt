file(GLOB_RECURSE CPPS  ./*.cpp )


SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(SOURCES NodeForm.cpp)
qt_wrap_cpp(SOURCES NodeForm.h)
qt_wrap_ui(UI_HEADERS NodeForm.ui)

add_executable(game ${CPPS} ${UI_HEADERS})
target_link_libraries(game nodes)
