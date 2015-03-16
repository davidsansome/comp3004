FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(SDL REQUIRED)
FIND_PACKAGE(SDL_image REQUIRED)

# Include our custom cmake modules
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

MACRO (WRAP_SHADERS outfiles )
	FOREACH (it ${ARGN})
		SET(infile "${CMAKE_CURRENT_SOURCE_DIR}/shaders/${it}")
		SET(outfile "${CMAKE_CURRENT_BINARY_DIR}/${it}")

		ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
			COMMAND cp
			ARGS ${infile} ${outfile}
			DEPENDS ${infile}
		)
		
		SET(${outfiles} ${${outfiles}} ${outfile})
	ENDFOREACH(it)
ENDMACRO(WRAP_SHADERS)

MACRO (WRAP_MODELS outfiles )
	FOREACH (it ${ARGN})
		SET(infile "${CMAKE_CURRENT_SOURCE_DIR}/models/${it}")
		SET(outfile "${CMAKE_CURRENT_BINARY_DIR}/${it}")

		ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
			COMMAND cp
			ARGS ${infile} ${outfile}
			DEPENDS ${infile}
		)
		
		SET(${outfiles} ${${outfiles}} ${outfile})
	ENDFOREACH(it)
ENDMACRO(WRAP_MODELS)

MACRO (WRAP_TEXTURES outfiles )
	FOREACH (it ${ARGN})
		SET(infile "${CMAKE_CURRENT_SOURCE_DIR}/textures/${it}")
		SET(outfile "${CMAKE_CURRENT_BINARY_DIR}/${it}")

		ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
			COMMAND cp
			ARGS ${infile} ${outfile}
			DEPENDS ${infile}
		)
		
		SET(${outfiles} ${${outfiles}} ${outfile})
	ENDFOREACH(it)
ENDMACRO(WRAP_TEXTURES)


# Source files
SET(CW3-SOURCES
	main.cpp
	engine.cpp
	model.cpp
	sdlglwindow.cpp
	shader.cpp
	utilities.cpp
	terrain.cpp
	texture.cpp
	timedependentvariable.cpp
	waterstuff.cpp
	tankwalls.cpp
	outside.cpp
	seaweed.cpp
	fish.cpp
	watersurface.cpp
	bezier.cpp
	castle.cpp
	glwin.cpp
	starfish.cpp
)

IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	SET(CW3-SOURCES
		${CW3-SOURCES}
		SDLMain.m
	)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

SET(CW3-SHADERS
	fish_frag.glsl
	fish_vert.glsl
	terrain_frag.glsl
	terrain_vert.glsl
	terrainr_frag.glsl
	terrainr_vert.glsl
	pp_frag_p0.glsl
	pp_frag_p1.glsl
	pp_frag_p2.glsl
	pp_vert_p0.glsl
	pp_vert_p1.glsl
	pp_vert_p2.glsl
	waterstuff_vert.glsl
	waterstuff_frag.glsl
	tank_vert.glsl
	tank_frag.glsl
	table_vert.glsl
	table_frag.glsl
	seaweed_vert.glsl
	seaweed_frag.glsl
	watersurface_vert.glsl
	watersurface_frag.glsl
	castle_vert.glsl
	castle_frag.glsl
	starfish_vert.glsl
	starfish_frag.glsl
	shadowvol_vert.glsl
	shadowvol_frag.glsl
)

SET(CW3-MODELS
	terrorfish.mdl
	seaweed.mdl
	castle.mdl
	starfish.mdl
)

SET(CW3-TEXTURES
	sand.jpg
	moss.jpg
	noise.jpg
	glow.jpg
	wood.jpg
	seaweed.jpg
	caust0.jpg
	caust1.jpg
	caust2.jpg
	caust3.jpg
	caust4.jpg
	caust5.jpg
	caust6.jpg
	caust7.jpg
	caust8.jpg
	caust9.jpg
	caust10.jpg
	caust11.jpg
	caust12.jpg
	caust13.jpg
	caust14.jpg
	caust15.jpg
	caust16.jpg
	caust17.jpg
	caust18.jpg
	caust19.jpg
	caust20.jpg
	caust21.jpg
	caust22.jpg
	caust23.jpg
	caust24.jpg
	caust25.jpg
	caust26.jpg
	caust27.jpg
	caust28.jpg
	caust29.jpg
	caust30.jpg
	caust31.jpg
	darkstone.jpg
	lightstone.jpg
	brick.jpg
	starfish.jpg
	help.jpg
)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${OPENGL_INCLUDE_DIR}
	${SDL_INCLUDE_DIR}
	${SDLIMAGE_INCLUDE_DIR}
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

WRAP_SHADERS(CW3-GLSL-SOURCES ${CW3-SHADERS})
WRAP_MODELS(CW3-MODEL-SOURCES ${CW3-MODELS})
WRAP_TEXTURES(CW3-TEXTURE-SOURCES ${CW3-TEXTURES})
ADD_EXECUTABLE(cw3 ${CW3-SOURCES} ${CW3-GLSL-SOURCES} ${CW3-MODEL-SOURCES} ${CW3-TEXTURE-SOURCES})

TARGET_LINK_LIBRARIES(cw3
	${OPENGL_LIBRARIES}
	${SDLMAIN_LIBRARY}
	${SDL_LIBRARY}
	${SDLIMAGE_LIBRARY}
	-lsvl
)
