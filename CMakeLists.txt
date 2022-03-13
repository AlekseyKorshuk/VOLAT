cmake_minimum_required(VERSION 3.17)
project(WGForgeProject)


set(CMAKE_CXX_STANDARD 17)

set(SOCKET SocketCompact)
set(JSON_VERSION "json-3.10.5")

include_directories(third-party/${JSON_VERSION}/include third-party/${SOCKET})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

IF (WIN32)
    # set stuff for windows
    link_libraries(ws2_32)
ELSE ()
    # set stuff for other systems
ENDIF ()


# Set all modules here
set(CLIENT
        modules/client/Client.cpp
        modules/client/Client.h
        )

set(CORE
        modules/core/Core.cpp
        modules/core/Core.h
        )

set(MAP
        modules/map/Hex.cpp
        modules/map/Hex.h
        modules/map/Map.cpp
        modules/map/Map.h
        )

set(CONTENT
        modules/content/Content.h
        modules/content/Content.cpp
        )

set(VEHICLES
        modules/content/vehicles/Tank.cpp
        modules/content/vehicles/Tank.h
        modules/content/vehicles/MediumTank.h
        modules/content/vehicles/MediumTank.cpp
        modules/content/vehicles/LightTank.h
        modules/content/vehicles/LightTank.cpp
        modules/content/vehicles/HeavyTank.h
        modules/content/vehicles/HeavyTank.cpp
        modules/content/vehicles/Spg.h
        modules/content/vehicles/Spg.cpp
        modules/content/vehicles/AtSpg.h
        modules/content/vehicles/AtSpg.cpp
        )

SET(STATE
        modules/state/State.cpp
        modules/state/State.h
        modules/state/StateCapture.cpp
        modules/state/StateCapture.h
        modules/state/StateHunt.cpp
        modules/state/StateHunt.h
        modules/state/StateDefence.cpp
        modules/state/StateDefence.h
        modules/state/StateHealth.cpp
        modules/state/StateHealth.h
        )

SET(STATE_MACHINE
        modules/state_machine/StateMachine.cpp
        modules/state_machine/StateMachine.h

        )
SET(GAME
        modules/game/Game.cpp
        modules/game/Game.h
        )

SET(PLAYER
        modules/player/Player.cpp
        modules/player/Player.h
        )

set(STRATEGY
        modules/strategy/Strategy.cpp
        modules/strategy/Strategy.h
        modules/state_machine/MediumTankStrategy.cpp
        modules/state_machine/MediumTankStrategy.h
        modules/state_machine/AtSpgStrategy.cpp
        modules/state_machine/AtSpgStrategy.h
        modules/state_machine/HeavyTankStrategy.cpp
        modules/state_machine/HeavyTankStrategy.h
        modules/state_machine/LightTankStrategy.cpp
        modules/state_machine/LightTankStrategy.h
        modules/state_machine/SpgStrategy.cpp
        modules/state_machine/SpgStrategy.h

        modules/state/StateCamping.cpp modules/state/StateCamping.h  modules/map/Position.cpp modules/map/Position.h)


add_executable(
        WGForgeProject
        main.cpp

        #how_to_use_map.cpp

        # Add all modules here
        ${CLIENT}
        ${CORE}
        ${STRATEGY}
        ${MAP}
        ${CONTENT}
        ${VEHICLES}
        ${STATE}
        ${STATE_MACHINE}
        ${GAME}
        ${PLAYER}
)
target_link_libraries(WGForgeProject PRIVATE Threads::Threads)

