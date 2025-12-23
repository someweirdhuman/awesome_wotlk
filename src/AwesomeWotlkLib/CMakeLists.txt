project( AwesomeWotlkLib )

set(SOURCES
    Entry.cpp
    GameClient.h
    NamePlates.h NamePlates.cpp
    Hooks.h Hooks.cpp
    Misc.h Misc.cpp
    BugFixes.h BugFixes.cpp
    Utils.h Utils.cpp
    CommandLine.cpp CommandLine.h
    Inventory.cpp Inventory.h
    Item.h Item.cpp
    UnitAPI.h UnitAPI.cpp
    Spell.h Spell.cpp
    VoiceChat.h VoiceChat.cpp
)

if(ENABLE_SMOOTH_FONTS)
	list(APPEND SOURCES Fonts.h Fonts.cpp)
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(
    ${PROJECT_NAME} PRIVATE
		${CMAKE_SOURCE_DIR}/deps
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
        Detours
        ole32
        oleaut32
        uuid
        sapi
)

if(ENABLE_SMOOTH_FONTS)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
		msdfgen::msdfgen-core
		msdfgen::msdfgen-ext
		freetype
		"${CMAKE_SOURCE_DIR}/deps/skia/skia.lib"
	)

	target_include_directories(
		${PROJECT_NAME} PRIVATE
			${CMAKE_SOURCE_DIR}/deps/skia/include
			${CMAKE_SOURCE_DIR}/deps/msdfgen
			${CMAKE_BINARY_DIR}/deps/msdfgen
	)

    target_compile_definitions(AwesomeWotlkLib PRIVATE ENABLE_SMOOTH_FONTS)
endif()


target_compile_definitions(
    ${PROJECT_NAME} PRIVATE
        APP_NAME="${PROJECT_NAME}"
)

if(TARGET AwesomeWotlkLib)
    if(ENABLE_SMOOTH_FONTS)
        target_compile_definitions(AwesomeWotlkLib PRIVATE ENABLE_SMOOTH_FONTS)
    endif()
endif()