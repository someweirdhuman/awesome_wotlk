cmake_minimum_required(VERSION 3.15)
project(AwesomeWotlkPatch)

add_executable(
    ${PROJECT_NAME}
    Main.cpp
    Utils.h
    Patch.h
)

target_compile_definitions(
    ${PROJECT_NAME} PRIVATE
    APP_NAME="${PROJECT_NAME}"
)

#uac escalation doesnt allow console output, need to remove escalation to get proper output
#removing escalation may fail in some instances where player has client in C: 
if(MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE
        "/MANIFESTUAC:level='requireAdministrator' uiAccess='false'"
        "/SUBSYSTEM:WINDOWS" 
        "/ENTRY:mainCRTStartup" 
    )
endif()