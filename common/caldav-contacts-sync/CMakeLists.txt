include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

set(kaccounts_caldav_plugin_SRCS
    caldav-plugin.cpp
)

add_library(kaccounts_caldav_plugin MODULE ${kaccounts_carddav_plugin_SRCS})

target_link_libraries(kaccounts_caldav_plugin
                      Qt5::Core
                      Qt5::Network
                      Qt5::Gui
                      Qt5::Xml
                      KF5::CoreAddons
                      KF5::ConfigCore
                      KAccounts
                      ${SIGNONQT_LIBRARIES}
)

install (TARGETS kaccounts_caldav_plugin
         DESTINATION ${PLUGIN_INSTALL_DIR}/kaccounts/daemonplugins
)


