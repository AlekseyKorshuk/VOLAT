cmake_minimum_required(VERSION 3.17)
project(WGForgeProject)

set(CMAKE_CXX_STANDARD 17)

set(JSON_VERSION "json-3.10.5")
include_directories(third-party/${JSON_VERSION}/include)


link_libraries(ws2_32)


# Set all modules here
set(CLIENT
        modules/client/Client.cpp
        modules/client/Client.h
        )

set(CORE
        modules/core/Core.cpp
        modules/core/Core.h
        )

set(STRATEGY
        modules/strategy/Strategy.cpp
        modules/strategy/Strategy.h
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
        )

SET(STATE
        modules/state/State.cpp
        modules/state/State.h
        modules/state/StateCapture.cpp
        modules/state/StateCapture.h
        )

SET(STATE_MACHINE
        modules/state_machine/StateMachine.cpp
        modules/state_machine/StateMachine.h
        modules/state_machine/MediumTankStrategy.cpp
        modules/state_machine/MediumTankStrategy.h)

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
)
