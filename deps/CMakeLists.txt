include(FetchContent)

set(LIBLCF_VERSION 0.8)
set(QT_VERSION v5.15.11-lts-lgpl)

FetchContent_Declare(
  liblcf
  URL "https://github.com/EasyRPG/liblcf/archive/${LIBLCF_VERSION}.zip"
  FIND_PACKAGE_ARGS NAMES lcf
)

FetchContent_MakeAvailable(liblcf)

FetchContent_Declare(
  qt5
  URL "https://github.com/qt/qtbase/archive/refs/tags/${QT_VERSION}.zip"
  FIND_PACKAGE_ARGS
    NAMES Qt5
    COMPONENTS QT_FEATURE_widgets
)


FetchContent_MakeAvailable(qt5)
