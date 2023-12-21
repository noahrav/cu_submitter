include(FetchContent)

set(LIBLCF_VERSION 0.8)
set(PISTACHE_VERSION 0.0.5)

find_package(liblcf)

if (NOT liblcf_FOUND)
  FetchContent_Declare(
    liblcf
    URL "https://github.com/EasyRPG/liblcf/archive/${LIBLCF_VERSION}.zip"
    FIND_PACKAGE_ARGS NAMES lcf
  )

  FetchContent_MakeAvailable(liblcf)
endif()

#find_package(pistache)

if (NOT pistache_FOUND)
  FetchContent_Declare(
    pistache
    URL "https://github.com/pistacheio/pistache/archive/${PISTACHE_VERSION}.zip"
  )

  FetchContent_MakeAvailable(pistache)
endif()
