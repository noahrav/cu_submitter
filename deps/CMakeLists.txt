include(FetchContent)

set(LIBLCF_VERSION 0.8)
set(QT_VERSION v5.15.8-lts-lgpl)

FetchContent_Declare(
  liblcf
  URL "https://github.com/EasyRPG/liblcf/archive/${LIBLCF_VERSION}.zip"
  FIND_PACKAGE_ARGS NAMES lcf
)

FetchContent_Declare(
  qt5
  URL "https://github.com/EasyRPG/liblcf/archive/${QT_VERSION}.zip"
  GIT_TAG        master
  FIND_PACKAGE_ARGS NAMES Qt5::Widgets
)

FetchContent_MakeAvailable(liblcf qt5)
