project(WarhammerSim)

add_library(
        whSim
        Dice.cpp
        Unit.cpp
        Weapon.cpp
        Model.cpp
        Player.cpp
        Roster.cpp
        Battle.cpp
        Board.cpp
        TerrainFeature.cpp
        stormcast/Liberators.cpp
        stormcast/Judicators.cpp
        stormcast/CelestarBallista.cpp
        khorne/Bloodreavers.cpp
        sylvaneth/Alarielle.cpp
)

target_include_directories(
        whSim PUBLIC
        ../include
)
