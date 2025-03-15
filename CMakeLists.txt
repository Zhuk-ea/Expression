cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Expression")

add_library (Expression SHARED src/Expression.cpp include/Expression.h include/Tokenizer.h src/Tokenizer.cpp)


target_include_directories(Expression PRIVATE
    include
)

add_executable(tests
    include/Tests.h
    src/Tests.cpp
    src/main.cpp
)

target_link_libraries(tests PRIVATE
    Expression
)


target_include_directories(tests PRIVATE
    include
)

add_custom_target(test ./tests DEPENDS tests)