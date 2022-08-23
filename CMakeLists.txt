cmake_minimum_required(VERSION 3.12)

project(gold_framework)

file(GLOB_RECURSE GOLD_SRC ${PROJECT_SOURCE_DIR}/gold/*.cpp ${PROJECT_SOURCE_DIR}/gold/*.h)
file(GLOB_RECURSE DEMO_SRC ${PROJECT_SOURCE_DIR}/demo/*.cpp ${PROJECT_SOURCE_DIR}/demo/*.h)
add_executable(gold_framework WIN32 ${GOLD_SRC} ${DEMO_SRC})

set_property(TARGET gold_framework PROPERTY CXX_STANDARD 20)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(gold_framework PRIVATE -DUNICODE -D_UNICODE -DNOMINMAX /Zi)
    target_link_options(gold_framework PRIVATE /DEBUG /OPT:REF /OPT:ICF)
else()
    target_compile_options(gold_framework PRIVATE -municode -fpermissive)
endif()

target_include_directories(gold_framework PRIVATE ${PROJECT_SOURCE_DIR} vendor/)
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_link_directories(gold_framework PRIVATE lib)
    target_link_libraries(gold_framework PRIVATE opengl32 glew32 glu32 gdi32 glmf32 lua54)
else()
    target_link_libraries(gold_framework PRIVATE opengl32 :libglew32.dll.a glu32 gdi32 glmf32 :libturbojpeg.a :liblua54.dll.a)
endif()
target_link_options(gold_framework PRIVATE -static -pthread)