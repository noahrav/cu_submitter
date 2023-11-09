include(FetchContent)

set(LIBLCF_VERSION 0.8)

FetchContent_Declare(
  liblcf
  URL "https://github.com/EasyRPG/liblcf/archive/${LIBLCF_VERSION}.zip"
  FIND_PACKAGE_ARGS NAMES lcf
)

FetchContent_Declare(
  qt5
  GIT_REPOSITORY https://github.com/qt/qt5.git
  GIT_TAG        master
  FIND_PACKAGE_ARGS NAMES Qt5::Widgets
)

FetchContent_MakeAvailable(liblcf qt5)
