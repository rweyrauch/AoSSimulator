project(WarhammerSim)

set(CMAKE_CXX_STANDARD 20)

file(GLOB stormcast_src "stormcast/*.cpp" "stormcast/*.h")
file(GLOB khorne_src "khorne/*.cpp" "khorne/*.h")
file(GLOB gloomspitegitz_src "gloomspitegitz/*.cpp" "gloomspitegitz/*.h")
file(GLOB citiesofsigmar_src "citiesofsigmar/*.cpp" "citiesofsigmar/*.h")
file(GLOB slavestodarkness_src "slavestodarkness/*.cpp" "slavestodarkness/*.h")
file(GLOB skaven_src "skaven/*.cpp" "skaven/*.h")
file(GLOB mawtribes_src "mawtribes/*.cpp" "mawtribes/*.h")
file(GLOB beastsofchaos_src "beastsofchaos/*.cpp" "beastsofchaos/*.h")
file(GLOB nurgle_src "nurgle/*.cpp" "nurgle/*.h")
file(GLOB seraphon_src "seraphon/*.cpp" "seraphon/*.h")
file(GLOB sylvaneth_src "sylvaneth/*.cpp" "sylvaneth/*.h")
file(GLOB dok_src "dok/*.cpp" "dok/*.h")
file(GLOB idonethdeepkin_src "idonethdeepkin/*.cpp" "idonethdeepkin/*.h")
file(GLOB nighthaunt_src "nighthaunt/*.cpp" "nighthaunt/*.h")
file(GLOB slaanesh_src "slaanesh/*.cpp" "slaanesh/*.h")
file(GLOB death_src "death/*.cpp" "death/*.h")
file(GLOB fec_src "fec/*.cpp" "fec/*.h")
file(GLOB tzeentch_src "tzeentch/*.cpp" "tzeentch/*.h")
file(GLOB ironjawz_src "ironjawz/*.cpp" "ironjawz/*.h")
file(GLOB bonesplitterz_src "bonesplitterz/*.cpp" "bonesplitterz/*.h")
file(GLOB fyreslayers_src "fyreslayers/*.cpp" "fyreslayers/*.h")
file(GLOB ossiarch_src "ossiarch/*.cpp" "ossiarch/*.h")
file(GLOB kharadron_src "kharadron/*.cpp" "kharadron/*.h")
file(GLOB lumineth_src "lumineth/*.cpp" "lumineth/*.h")
file(GLOB soulblight_src "soulblight/*.cpp" "soulblight/*.h")

set(AOSSIM_SOURCE
        Dice.cpp
        Unit.cpp
        Weapon.cpp
        Model.cpp
        Player.cpp
        Roster.cpp
        Battle.cpp
        Board.cpp
        TerrainFeature.cpp
        ManoAMano.cpp
        AgeOfSigmarSim.cpp
        Spell.cpp
        UnitFactory.cpp
        UnitInit.cpp
        MathUtils.cpp
        UnitStatistics.cpp
        CommandAbility.cpp
        Prayer.cpp
        Think.cpp
        Game.cpp
        Battleshock.cpp
        Combat.cpp
        Charge.cpp
        Shooting.cpp
        Movement.cpp
        Hero.cpp
        #jps.hh
        #pathfinders.cpp
        ../include/UnitModifierInterface.h
        ../include/UnitDecorator.h

        spells/MysticShield.cpp

        ${stormcast_src}
        ${khorne_src}
        ${gloomspitegitz_src}
        ${citiesofsigmar_src}
        ${slavestodarkness_src}
        ${skaven_src}
        ${mawtribes_src}
        ${beastsofchaos_src}
        ${nurgle_src}
        ${seraphon_src}
        ${sylvaneth_src}
        ${dok_src}
        ${idonethdeepkin_src}
        ${nighthaunt_src}
        ${slaanesh_src}
        ${death_src}
        ${fec_src}
        ${tzeentch_src}
        ${ironjawz_src}
        ${bonesplitterz_src}
        ${fyreslayers_src}
        ${ossiarch_src}
        ${kharadron_src}
        ${lumineth_src}
        ${soulblight_src}

        wanderers/Wanderer.cpp
        wanderers/EternalGuard.cpp
        wanderers/GladeGuard.cpp
        wanderers/WildwoodRangers.cpp

        eldritchcouncil/Swordmasters.cpp
        eldritchcouncil/Loremaster.cpp
        eldritchcouncil/Archmage.cpp
        eldritchcouncil/ArchmageOnDragon.cpp

        greenskinz/Orruks.cpp
        greenskinz/OrrukBoarboys.cpp
        greenskinz/OrrukBoarChariots.cpp
        greenskinz/OrrukGreatShaman.cpp
        greenskinz/OrrukWarboss.cpp
        greenskinz/OrrukWarbossWyvern.cpp
        greenskinz/RogueIdol.cpp

        everchosen/Archaon.cpp
        everchosen/Varanguard.cpp

        freeagents/GotrekGurnisson.cpp
        freeagents/Kragnos.cpp

        behemat/SonsOfBehemat.cpp
        behemat/Mancrusher.cpp
        behemat/Gatebreaker.cpp
        behemat/KrakenEater.cpp
        behemat/Warstomper.cpp
        )

add_library(
        AoSSim STATIC
        ${AOSSIM_SOURCE}
)

if (NOT EMSCRIPTEN)
    target_include_directories(
            AoSSim PUBLIC
            ${GTK3_INCLUDE_DIRS}
            ../include
    )
    target_compile_options(
            AoSSim PRIVATE
            -Wpedantic
            -Wextra
            -Wall
            -Wformat-security
            -Wno-unused-parameter
    )
    target_link_libraries(
            AoSSim
            ${GTK3_LIBRARIES}
    )
    target_compile_definitions(
            AoSSim PUBLIC HAVE_CAIRO
    )
else ()
    target_include_directories(
            AoSSim PUBLIC
            ../include
    )
    target_compile_options(
            AoSSim PRIVATE
            -Wpedantic
            -Wextra
            -Wall
            -Wformat-security
            -Wno-unused-parameter
            -fbracket-depth=257
    )

endif ()

