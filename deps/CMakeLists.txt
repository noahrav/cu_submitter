include(FetchContent)

set(LIBLCF_VERSION 0.8)
set(QT_VERSION v5.15.11-lts-lgpl)

FetchContent_Declare(
  liblcf
  URL "https://github.com/EasyRPG/liblcf/archive/${LIBLCF_VERSION}.zip"
  FIND_PACKAGE_ARGS NAMES lcf
)

FetchContent_Declare(
  Qt5
  URL "https://github.com/qt/qt5/archive/refs/tags/${QT_VERSION}.zip"
  FIND_PACKAGE_ARGS NAMES Qt5 COMPONENTS Widgets
)

FetchContent_MakeAvailable(liblcf Qt5)
