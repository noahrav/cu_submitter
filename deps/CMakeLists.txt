include(FetchContent)

set(LIBLCF_VERSION 0.8)
set(BUILD_LIBLCF ON)

if (BUILD_LIBLCF)
  FetchContent_Declare(
    liblcf
    URL "https://github.com/EasyRPG/liblcf/archive/${LIBLCF_VERSION}.zip"
    FIND_PACKAGE_ARGS NAMES lcf
  )

  FetchContent_MakeAvailable(liblcf)
else()
  find_package(liblcf REQUIRED)
endif()
