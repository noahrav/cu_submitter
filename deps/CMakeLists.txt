include(FetchContent)

set(LIBLCF_VERSION 0.8)
set(PISTACHE_VERSION 0.0.5)
set(RAPIDJSON_VERSION 1.1.0)

find_package(liblcf)

if (NOT liblcf_FOUND)
  FetchContent_Declare(
    liblcf
    URL "https://github.com/EasyRPG/liblcf/archive/${LIBLCF_VERSION}.zip"
    FIND_PACKAGE_ARGS NAMES lcf
  )

  FetchContent_MakeAvailable(liblcf)
endif()

if (NOT pistache_FOUND)
  FetchContent_Declare(
    pistache
    URL "https://github.com/pistacheio/pistache/archive/${PISTACHE_VERSION}.zip"
  )

  FetchContent_MakeAvailable(pistache)
endif()

FetchContent_Declare(
  rapidjson
  URL "https://github.com/Tencent/rapidjson/archive/v${RAPIDJSON_VERSION}.zip"
)

FetchContent_MakeAvailable(rapidjson)
