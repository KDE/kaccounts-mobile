project(KAccounts-mobile)

cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 1.0.0 REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

set(REQUIRED_QT_VERSION "5.2.0")
set(REQUIRED_KF5_VERSION "5.4.0")
set(ACCOUNTSQT_DEP_VERSION "1.10")
set(SIGNONQT_DEP_VERSION "8.55")

find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core Widgets Qml Xml)
find_package(KF5 ${REQUIRED_KF5_VERSION} REQUIRED KCMUtils KIO I18n WidgetsAddons CoreAddons
                                                  IconThemes Config Wallet DBusAddons Declarative Package)
find_package(AccountsQt5 ${ACCOUNTSQT_DEP_VERSION} REQUIRED CONFIG)
find_package(SignOnQt5 ${SIGNONQT_DEP_VERSION} REQUIRED CONFIG)

add_definitions (-fexceptions
                 -DQT_NO_KEYWORDS)

find_package(KF5Contacts)
find_package(KF5GAPI)
find_package(Qt5WebKitWidgets) #needed for kgapi apparently

add_definitions(-DTRANSLATION_DOMAIN=\"kaccounts-mobile\")

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings)
include(KDECompilerSettings)
include(ECMInstallIcons)
include(FeatureSummary)

include_directories(${ACCOUNTSQT_INCLUDE_DIRS} ${SIGNONQT_INCLUDE_DIRS})

add_subdirectory(google)
add_subdirectory(common)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
