cmake_minimum_required(VERSION 3.15)

project(
	Time_Shifters
	VERSION 0.1
	DESCRIPTION "A 2D game about a knight who can alter time"
	LANGUAGES CXX
)

set(SOURCES
	src/TS/Components/AttackBoxCollider.cpp
	src/TS/Components/AttackCollidable.cpp
	src/TS/Components/BaseBanditAI.cpp
	src/TS/Components/ModifiedBoxCollider.cpp
	src/TS/Components/ScreenText.cpp
	src/TS/Components/Updateable.cpp
	src/TS/Entities/Bandit.cpp
	src/TS/Entities/Character.cpp
	src/TS/Entities/Hero.cpp
	src/TS/Entities/SlideDust.cpp
	src/TS/Scenes/DemoLevel.cpp
	src/TS/Systems/UpdateableSystem.cpp
	src/TS/Utility/Define.cpp
	src/TS/main.cpp
)

set(SDL_LIBS
	SDL2
	SDL2_image
	SDL2_mixer
	SDL2_ttf
)

option(COMPILE_FOR_WIN, "An option to allow for compilation for windows machine" OFF)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_20)


set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

target_compile_options(compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-O3>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

add_executable(time_shifters ${SOURCES})

target_include_directories(time_shifters
	PUBLIC
	${PROJECT_SOURCE_DIR}/include/
)

target_link_libraries(time_shifters BA ${SDL_LIBS} compiler_flags)



#INSTALLATION
install(
	DIRECTORY ${PROJECT_SOURCE_DIR}/dirs/ DESTINATION share
)
install(
	TARGETS time_shifters
	DESTINATION bin
)
