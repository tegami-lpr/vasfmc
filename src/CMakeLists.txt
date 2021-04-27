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

find_package(vaslib REQUIRED)

file(GLOB VASFMC_SOURCES fmc_data.cpp fmc_control.cpp fmc_autopilot.cpp fmc_autothrottle.cpp fmc_processor.cpp
        fmc_navdisplay.cpp fmc_navdisplay_glwidget.cpp fmc_navdisplay_style.cpp fmc_navdisplay_style_a.cpp
        fmc_navdisplay_style_b.cpp gldraw.cpp opengltext.cpp lfontrenderer.cpp fmc_cdu.cpp fmc_cdu_style_a.cpp
        fmc_cdu_page_manager.cpp fmc_cdu_page_base.cpp fmc_cdu_page.cpp fmc_cdu_page_menu.cpp fmc_cdu_page_perf.cpp
        fmc_cdu_page_atc.cpp fmc_console.cpp fmc_pfd.cpp fmc_pfd_glwidget_base.cpp fmc_pfd_glwidget_style_a.cpp
        fmc_pfd_glwidget_style_b.cpp fmc_ecam.cpp fmc_ecam_glwidget_base.cpp fmc_ecam_glwidget_style_a.cpp
        fmc_ecam_glwidget_style_b.cpp fmc_sounds.cpp fmc_sounds_style_a.cpp fmc_sounds_style_b.cpp fmc_sounds_handler.cpp
        vas_widget.cpp vas_gl_widget.cpp fmc_fcu.cpp fmc_fcu_style_a.cpp mmx.cpp fmc_flightstatus_checker_base.cpp
        fmc_flightstatus_checker_style_a.cpp cpflight_serial.cpp iocp.cpp fmc_gps.cpp fmc_gps_style_g.cpp
        checklist.cpp main.cpp)

file(GLOB VASFMC_HEADERS defines.h fmc_data.h fmc_control_defines.h fmc_control.h fmc_autopilot.h fmc_autopilot_defines.h
        fmc_autothrottle.h fmc_autothrottle_defines.h fmc_processor.h fmc_navdisplay_defines.h fmc_navdisplay.h
        fmc_navdisplay_glwidget.h fmc_navdisplay_style.h fmc_navdisplay_style_a.h fmc_navdisplay_style_b.h
        gldraw.h opengltext.h lfontrenderer.h fmc_cdu.h fmc_cdu_defines.h fmc_cdu_style_a.h fmc_cdu_page_manager.h
        fmc_cdu_page_base.h fmc_cdu_page.h fmc_cdu_page_menu.h fmc_cdu_page_perf.h fmc_cdu_page_atc.h fmc_console_defines.h
        fmc_console.h fmc_pfd_defines.h fmc_pfd.h fmc_pfd_glwidget_base.h fmc_pfd_glwidget_style_a.h fmc_pfd_glwidget_style_b.h
        fmc_ecam_defines.h fmc_ecam.h fmc_ecam_glwidget_base.h fmc_ecam_glwidget_style_a.h fmc_ecam_glwidget_style_b.h fmc_sounds_defines.h
        fmc_sounds.h fmc_sounds_style_a.h fmc_sounds_style_b.h fmc_sounds_handler.h vas_widget.h vas_gl_widget.h fmc_fcu.h fmc_fcu_defines.h
        fmc_fcu_style_a.h mmx.h fmc_flightstatus_checker_base.h fmc_flightstatus_checker_style_a.h cpflight_serial.h iocp.h fmc_gps.h fmc_gps_defines.h
        fmc_gps_style_g.h checklist.h)

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

if(WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32)
endif()



################## FONT STUFF ##################
target_compile_definitions(${PROJECT_NAME} PRIVATE USE_FONTRENDERER)

################## SOUND STUFF ##################
target_compile_definitions(${PROJECT_NAME} PRIVATE USE_QSOUND)

target_compile_definitions(${PROJECT_NAME} PRIVATE VAS_GL_EMUL=0)