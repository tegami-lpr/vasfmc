set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC             ON)
set(CMAKE_AUTORCC             ON)
set(CMAKE_AUTOUIC             ON)

# Qt's components
find_package(Qt5 5.12 COMPONENTS Core Gui Network Widgets Xml REQUIRED)
set(QT_DEPS_LIBRARIES Qt5::Core Qt5::Gui Qt5::Network Qt5::Widgets Qt5::Xml)

if(WIN32)
    # Files to be included for P3D support
    file(GLOB P3D_SOURCES code_timer.cpp fsaccess_msfs.cpp fsuipc.cpp)
    file(GLOB P3D_HEADERS code_timer.h fsaccess_msfs.h fsuipc.h)

    if ("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        set(FSUIPC_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/FSUIPC64")
    else()
        set(FSUIPC_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/FSUIPC")
    endif()
    file(GLOB FSUIPC_SOURCES ${FSUIPC_INCLUDE_DIR}/IPCuser.c)
    file(GLOB FSUIPC_HEADERS ${FSUIPC_INCLUDE_DIR}/FSUIPC_User.h ${FSUIPC_INCLUDE_DIR}/IPCuser.h)
endif(WIN32)

if(XPLANE_SUPPORT)
    file(GLOB XPLANE_SOURCES fsaccess_xplane.cpp)
    file(GLOB XPLANE_HEADERS fsaccess_xplane_defines.h fsaccess_xplane.h canas.h)
endif(XPLANE_SUPPORT)

# Files to be included for reading XML
file(GLOB COMMONXML_SOURCES xml.model.cpp)
file(GLOB COMMONXML_HEADERS xml.model.h)

# Files to be included for vascharts
# XXX: not used anymore?
if(CHARTS_SUPPORT)
    set(CHARTS_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/charts")
    file(GLOB CHARTS_SOURCES ${CHARTS_INCLUDE_DIR}/chartwidget.cpp ${CHARTS_INCLUDE_DIR}/ge.cpp ${CHARTS_INCLUDE_DIR}/ge_projection.cpp
            ${CHARTS_INCLUDE_DIR}/ge_list.cpp ${CHARTS_INCLUDE_DIR}/ge_layers.cpp ${CHARTS_INCLUDE_DIR}/ge_rect.cpp
            ${CHARTS_INCLUDE_DIR}/ge_label.cpp ${CHARTS_INCLUDE_DIR}/ge_vor.cpp ${CHARTS_INCLUDE_DIR}/ge_ndb.cpp
            ${CHARTS_INCLUDE_DIR}/ge_airport.cpp ${CHARTS_INCLUDE_DIR}/ge_intersection.cpp ${CHARTS_INCLUDE_DIR}/ge_route.cpp
            ${CHARTS_INCLUDE_DIR}/treebase.cpp ${CHARTS_INCLUDE_DIR}/treebase_xml.cpp ${CHARTS_INCLUDE_DIR}/treebase_xml_chart.cpp
            ${CHARTS_INCLUDE_DIR}/chartmodel.cpp ${CHARTS_INCLUDE_DIR}/chart.xml.defines.cpp ${CHARTS_INCLUDE_DIR}/chart.elem.root.cpp
            ${CHARTS_INCLUDE_DIR}/chart.elem.info.cpp ${CHARTS_INCLUDE_DIR}/chart.elem.projection.cpp ${CHARTS_INCLUDE_DIR}/chart.elem.navaids.cpp
            ${CHARTS_INCLUDE_DIR}/chart.elem.routes.cpp ${CHARTS_INCLUDE_DIR}/chart.elem.texts.cpp
            ${CHARTS_INCLUDE_DIR}/chart.elem.navaid.cpp ${CHARTS_INCLUDE_DIR}/chart.elem.route.cpp ${CHARTS_INCLUDE_DIR}/chart.elem.text.cpp
            ${CHARTS_INCLUDE_DIR}/chart.elem.vor.cpp ${CHARTS_INCLUDE_DIR}/chart.elem.ndb.cpp ${CHARTS_INCLUDE_DIR}/chart.elem.airport.cpp
            ${CHARTS_INCLUDE_DIR}/chart.elem.intersection.cpp ${CHARTS_INCLUDE_DIR}/chartinfodlgimpl.cpp ${CHARTS_INCLUDE_DIR}/routedlgimpl.cpp
            ${CHARTS_INCLUDE_DIR}/ge_textrect.cpp ${CHARTS_INCLUDE_DIR}/waypointchoosedlgimpl.cpp ${CHARTS_INCLUDE_DIR}/textelemdlgimpl.cpp)
    file(GLOB CHARTS_HEADERS ${CHARTS_INCLUDE_DIR}/chartwidget.h ${CHARTS_INCLUDE_DIR}/ge.h ${CHARTS_INCLUDE_DIR}/ge_projection.h
            ${CHARTS_INCLUDE_DIR}/ge_list.h ${CHARTS_INCLUDE_DIR}/ge_layers.h ${CHARTS_INCLUDE_DIR}/ge_rect.h ${CHARTS_INCLUDE_DIR}/ge_label.h
            ${CHARTS_INCLUDE_DIR}/ge_vor.h ${CHARTS_INCLUDE_DIR}/ge_ndb.h ${CHARTS_INCLUDE_DIR}/ge_airport.h ${CHARTS_INCLUDE_DIR}/ge_intersection.h
            ${CHARTS_INCLUDE_DIR}/ge_route.h ${CHARTS_INCLUDE_DIR}/treebase.h ${CHARTS_INCLUDE_DIR}/treebase_xml.h ${CHARTS_INCLUDE_DIR}/treebase_xml_chart.h
            ${CHARTS_INCLUDE_DIR}/chartmodel.h ${CHARTS_INCLUDE_DIR}/chart.xml.defines.h ${CHARTS_INCLUDE_DIR}/chart.elem.root.h ${CHARTS_INCLUDE_DIR}/chart.elem.info.h
            ${CHARTS_INCLUDE_DIR}/chart.elem.projection.h ${CHARTS_INCLUDE_DIR}/chart.elem.navaids.h ${CHARTS_INCLUDE_DIR}/chart.elem.routes.h
            ${CHARTS_INCLUDE_DIR}/chart.elem.texts.h ${CHARTS_INCLUDE_DIR}/chart.elem.navaid.h ${CHARTS_INCLUDE_DIR}/chart.elem.route.h
            ${CHARTS_INCLUDE_DIR}/chart.elem.text.h ${CHARTS_INCLUDE_DIR}/chart.elem.vor.h ${CHARTS_INCLUDE_DIR}/chart.elem.ndb.h
            ${CHARTS_INCLUDE_DIR}/chart.elem.airport.h ${CHARTS_INCLUDE_DIR}/chart.elem.intersection.h ${CHARTS_INCLUDE_DIR}/chartinfodlgimpl.h
            ${CHARTS_INCLUDE_DIR}/waypointchoosedlgimpl.h ${CHARTS_INCLUDE_DIR}/anchor_latlon.h ${CHARTS_INCLUDE_DIR}/termination_callback.h
            ${CHARTS_INCLUDE_DIR}/routedlgimpl.h ${CHARTS_INCLUDE_DIR}/ge_textrect.h ${CHARTS_INCLUDE_DIR}/waypointchoosedlgimpl.h ${CHARTS_INCLUDE_DIR}/textelemdlgimpl.h)
    file(GLOB CHARTS_UI ${CHARTS_INCLUDE_DIR}/chart.info.dlg.ui ${CHARTS_INCLUDE_DIR}/routedlg.ui ${CHARTS_INCLUDE_DIR}/waypointchoosedlg.ui ${CHARTS_INCLUDE_DIR}/textelemdlg.ui)
endif(CHARTS_SUPPORT)

# The files constituting vaslib itself
file(GLOB VASLIB_SOURCES messagebus/fmcmessagebus.cpp
        vlassert.cpp config.cpp configwidget.cpp logger.cpp pushbutton.cpp mouse_input_area.cpp smoothing.cpp
        waypoint.cpp airport.cpp runway.cpp intersection.cpp ndb.cpp vor.cpp ils.cpp holding.cpp route.cpp airway.cpp procedure.cpp
        procedure_serialization.cpp sid.cpp star.cpp transition.cpp approach.cpp flightroute.cpp flightstatus.cpp fsaccess.cpp
        navcalc.cpp navdata.cpp geodata.cpp projection_mercator.cpp projection_greatcircle.cpp transport_layer_iface.cpp
        transport_layer_tcpclient.cpp transport_layer_tcpserver.cpp transport_layer_tcpserver_clientbuffer.cpp containerfactory.cpp
        containerbase.cpp infodlgimpl.cpp declination.cpp declination_geomag.cpp fwpath.cpp controller_throttle.cpp
        controller_speed.cpp fly_by_wire.cpp flight_mode_tracker.cpp statistics.cpp aircraft_data.cpp noise_generator.cpp
        vroute.cpp weather.cpp)
file(GLOB VASLIB_HEADERS messagebus/fmcmessagebus.h messagebus/eventqueue.h
        ptrlist.h vlassert.h config.h configwidget.h logger.h meanvalue.h median.h pushbutton.h
        mouse_input_area.h smoothing.h waypoint.h waypoint_hdg_to_alt.h waypoint_hdg_to_intercept.h airport.h
        runway.h intersection.h ndb.h vor.h ils.h holding.h route.h airway.h procedure.h procedure_serialization.h
        sid.h star.h transition.h approach.h flightroute.h flightstatus.h fsaccess.h navcalc.h navdata.h gshhs.h
        geodata.h projection.h projection_mercator.h projection_greatcircle.h transport_layer_iface.h
        transport_layer_tcpclient.h transport_layer_tcpserver.h transport_layer_tcpserver_clientbuffer.h containerfactory.h
        containerbase.h infodlgimpl.h declination.h declination_geomag.h vas_path.h trend.h controller_base.h
        controller_throttle.h controller_speed.h fly_by_wire.h flight_mode_tracker.h statistics.h aircraft_data.h serialization_iface.h
        fmc_data_provider.h bithandling.h noise_generator.h vroute.h weather.h)

file(GLOB VASLIB_UI infodlg.ui latlonpointdlg.ui pbdpointdlg.ui)

set_source_files_properties(${PROJECT_NAME}.rc PROPERTIES LANGUAGE RC)

add_library(${PROJECT_NAME} STATIC ${VASLIB_SOURCES} ${VASLIB_HEADERS} ${VASLIB_UI} ${P3D_HEADERS} ${P3D_SOURCES} ${FSUIPC_HEADERS} ${FSUIPC_SOURCES} ${UI_FILES} ${RC_FILES} )
#add_library(${PROJECT_NAME} SHARED ${VASLIB_SOURCES} ${VASLIB_HEADERS} ${VASLIB_UI} ${P3D_HEADERS} ${P3D_SOURCES} ${FSUIPC_FILES} ${UI_FILES} ${RC_FILES} )

target_link_libraries(${PROJECT_NAME} ${QT_DEPS_LIBRARIES})

target_include_directories(${PROJECT_NAME} PRIVATE ${FSUIPC_INCLUDE_DIR})
#target_include_directories(${PROJECT_NAME} PUBLIC $<INSTALL_INTERFACE:${VL_INCLUDE_DIR}>)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/messagebus ${FSUIPC_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_autogen/include)


#install(TARGETS ${PROJECT_NAME} EXPORT vaslibTargets
#        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${VL_BIN_DIR}
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${VL_LIB_DIR}
#        COMPONENT lib)
#install(FILES   ${VASLIB_HEADERS}
#                ${P3D_HEADERS}
#                ${FSUIPC_HEADERS}
#                ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_autogen/include/ui_infodlg.h
#        DESTINATION ${CMAKE_INSTALL_PREFIX}/${VL_INCLUDE_DIR}
#        COMPONENT devel)

