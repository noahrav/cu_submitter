include(FetchContent)

set(LIBLCF_VERSION 0.8)

FetchContent_Declare(
  liblcf
  URL "https://github.com/EasyRPG/liblcf/archive/${LIBLCF_VERSION}.zip"
)

FetchContent_MakeAvailable(liblcf)
