include(FetchContent)
set(FETCHCONTENT_QUIET FALSE CACHE INTERNAL "")
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        5.5
  GIT_PROGRESS   ON
  GIT_SHALLOW    1
  )
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(raylib)
target_compile_options(raylib PRIVATE "-w")
