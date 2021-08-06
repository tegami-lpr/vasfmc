set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# The files constituting vaslib itself
file(GLOB FWLIB_SOURCES messagebus/fmcmessagebus.cpp *.cpp)
file(GLOB FWLIB_HEADERS messagebus/fmcmessagebus.h messagebus/eventqueue.h *.h)

add_library(${PROJECT_NAME} STATIC ${FWLIB_SOURCES} ${FWLIB_HEADERS})
target_link_libraries(${PROJECT_NAME} Userenv)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/messagebus)

