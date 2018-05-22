project(KAccounts-mobile)

cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 1.0.0 REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

set(REQUIRED_QT_VERSION "5.2.0")
set(REQUIRED_KF5_VERSION "5.4.0")
set(ACCOUNTSQT_DEP_VERSION "1.10")
set(SIGNONQT_DEP_VERSION "8.55")

find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core Xml)
find_package(KF5 ${REQUIRED_KF5_VERSION} REQUIRED KIO I18n CoreAddons Config)
find_package(KAccounts REQUIRED)
find_package(AccountsQt5 ${ACCOUNTSQT_DEP_VERSION} REQUIRED CONFIG)
find_package(SignOnQt5 ${SIGNONQT_DEP_VERSION} REQUIRED CONFIG)

find_package(KF5Contacts)
find_package(KPimGAPI)
find_package(Qt5WebKitWidgets) #needed for kgapi apparently

add_definitions(-DTRANSLATION_DOMAIN=\"kaccounts-mobile\")

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings)
include(KDECompilerSettings)
include(ECMInstallIcons)
include(FeatureSummary)

include_directories(${ACCOUNTSQT_INCLUDE_DIRS} ${SIGNONQT_INCLUDE_DIRS})
remove_definitions(-DQT_NO_CAST_FROM_ASCII)

add_subdirectory(google)
add_subdirectory(common)
add_subdirectory(services)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
