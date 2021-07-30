set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC             ON)
set(CMAKE_AUTORCC             ON)
set(CMAKE_AUTOUIC             ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_LIST_DIR}/ui)

# Qt's components
find_package(Qt5 COMPONENTS Core Gui OpenGL Multimedia Network Widgets Xml REQUIRED)
set(QT_DEPS_LIBRARIES Qt5::Core Qt5::Gui Qt5::OpenGL Qt5::Multimedia Qt5::Network Qt5::Widgets Qt5::Xml)

find_package(OpenGL)

#find_package(vaslib REQUIRED)

file(GLOB VASFMC_SOURCES fmc_data.cpp fmc_control.cpp fmc_autopilot.cpp fmc_autothrottle.cpp fmc_processor.cpp
        navdisplay/fmc_navdisplay.cpp navdisplay/fmc_navdisplay_glwidget.cpp navdisplay/fmc_navdisplay_style.cpp navdisplay/fmc_navdisplay_style_a.cpp
        navdisplay/fmc_navdisplay_style_b.cpp gldraw.cpp opengltext.cpp lfontrenderer.cpp cdu/fmc_cdu.cpp cdu/fmc_cdu_style_a.cpp
        cdu/fmc_cdu_page_manager.cpp cdu/fmc_cdu_page_base.cpp cdu/fmc_cdu_page.cpp cdu/fmc_cdu_page_menu.cpp cdu/fmc_cdu_page_perf.cpp
        cdu/fmc_cdu_page_atc.cpp fmc_console.cpp pfd/fmc_pfd.cpp pfd/fmc_pfd_glwidget_base.cpp pfd/fmc_pfd_glwidget_style_a.cpp
        pfd/fmc_pfd_glwidget_style_b.cpp ecam/fmc_ecam.cpp ecam/fmc_ecam_glwidget_base.cpp ecam/fmc_ecam_glwidget_style_a.cpp
        ecam/fmc_ecam_glwidget_style_b.cpp sounds/fmc_sounds.cpp sounds/fmc_sounds_style_a.cpp sounds/fmc_sounds_style_b.cpp sounds/fmc_sounds_handler.cpp
        vas_widget.cpp vas_gl_widget.cpp mmx.cpp fmc_flightstatus_checker_base.cpp
        fmc_flightstatus_checker_style_a.cpp cpflight_serial.cpp iocp.cpp gps/fmc_gps.cpp gps/fmc_gps_style_g.cpp
        checklist.cpp main.cpp
        fcu/fmc_fcu.cpp fcu/fmc_fcu_style_a.cpp fcu/fmc_fcu_style_a_m.cpp
        )

file(GLOB VASFMC_HEADERS defines.h fmc_data.h fmc_control_defines.h fmc_control.h fmc_autopilot.h fmc_autopilot_defines.h
        fmc_autothrottle.h fmc_autothrottle_defines.h fmc_processor.h navdisplay/fmc_navdisplay_defines.h navdisplay/fmc_navdisplay.h
        navdisplay/fmc_navdisplay_glwidget.h navdisplay/fmc_navdisplay_style.h navdisplay/fmc_navdisplay_style_a.h navdisplay/fmc_navdisplay_style_b.h
        gldraw.h opengltext.h lfontrenderer.h cdu/fmc_cdu.h cdu/fmc_cdu_defines.h cdu/fmc_cdu_style_a.h cdu/fmc_cdu_page_manager.h
        cdu/fmc_cdu_page_base.h cdu/fmc_cdu_page.h cdu/fmc_cdu_page_menu.h cdu/fmc_cdu_page_perf.h cdu/fmc_cdu_page_atc.h fmc_console_defines.h
        fmc_console.h pfd/fmc_pfd_defines.h pfd/fmc_pfd.h pfd/fmc_pfd_glwidget_base.h pfd/fmc_pfd_glwidget_style_a.h pfd/fmc_pfd_glwidget_style_b.h
        ecam/fmc_ecam_defines.h ecam/fmc_ecam.h ecam/fmc_ecam_glwidget_base.h ecam/fmc_ecam_glwidget_style_a.h ecam/fmc_ecam_glwidget_style_b.h sounds/fmc_sounds_defines.h
        sounds/fmc_sounds.h sounds/fmc_sounds_style_a.h sounds/fmc_sounds_style_b.h sounds/fmc_sounds_handler.h vas_widget.h vas_gl_widget.h
        mmx.h fmc_flightstatus_checker_base.h fmc_flightstatus_checker_style_a.h cpflight_serial.h iocp.h gps/fmc_gps.h gps/fmc_gps_defines.h
        gps/fmc_gps_style_g.h checklist.h
        fcu/fmc_fcu_defines.h fcu/fmc_fcu.h fcu/fmc_fcu_style_a.h fcu/fmc_fcu_style_a_m.h
        )

file(GLOB VASFMC_UI ${CMAKE_CURRENT_LIST_DIR}/ui/fmc_navdisplay.ui
                    ${CMAKE_CURRENT_LIST_DIR}/ui/fmc_pfd.ui
                    ${CMAKE_CURRENT_LIST_DIR}/ui/fmc_ecam.ui
                    ${CMAKE_CURRENT_LIST_DIR}/ui/fmc_cdu.ui
                    ${CMAKE_CURRENT_LIST_DIR}/ui/fmc_console.ui
                    ${CMAKE_CURRENT_LIST_DIR}/ui/fmc_fcu.ui
                    ${CMAKE_CURRENT_LIST_DIR}/ui/fmc_gps.ui)


if(VAS_GL_EMUL)
    file(GLOB GL_SOURCES vas_gl.cpp vas_gl_backend_qt.cpp vas_gl_backend_agg.cpp)
    file(GLOB GL_HEADERS vas_gl_backend_qt.h vas_gl_backend_agg.h)
else()
    file(GLOB GL_SOURCES vas_gl_native.cpp)
endif(VAS_GL_EMUL)

set_source_files_properties(${PROJECT_NAME}.rc PROPERTIES LANGUAGE RC)

add_executable(${PROJECT_NAME} WIN32 ${VASFMC_UI} ${VASFMC_SOURCES} ${VASFMC_HEADER} ${GL_SOURCES} ${GL_HEADERS})
target_link_libraries(${PROJECT_NAME} ${QT_DEPS_LIBRARIES} vaslib ${OPENGL_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/fcu)
#target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../vaslib/src ${CMAKE_CURRENT_LIST_DIR}/../../vaslib/src/messagebus)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32)
endif()



################## FONT STUFF ##################
target_compile_definitions(${PROJECT_NAME} PRIVATE USE_FONTRENDERER)

################## SOUND STUFF ##################
target_compile_definitions(${PROJECT_NAME} PRIVATE USE_QSOUND)

target_compile_definitions(${PROJECT_NAME} PRIVATE VAS_GL_EMUL=0)