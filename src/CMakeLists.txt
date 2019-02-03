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
        ManoAMano.cpp
        WarhammerSim.cpp
        Spell.cpp
        UnitFactory.cpp
        UnitInit.cpp
        MathUtils.cpp
        UnitStatistics.cpp

        stormcast/StormcastEternals.cpp
        stormcast/Liberators.cpp
        stormcast/Judicators.cpp
        stormcast/CelestarBallista.cpp
        stormcast/LordOrdinator.cpp
        stormcast/Retributors.cpp
        stormcast/Sequitors.cpp
        stormcast/Evocators.cpp
        stormcast/LordCelestant.cpp
        stormcast/LordCelestantOnDracoth.cpp
        stormcast/LordCelestantOnStardrake.cpp
        stormcast/KnightIncantor.cpp
        stormcast/KnightQuestor.cpp
        stormcast/KnightZephyros.cpp
        stormcast/VanguardHunters.cpp
        stormcast/VanguardRaptorsHurricane.cpp
        stormcast/VanguardRaptorsLongstrike.cpp
        stormcast/Castigators.cpp

        khorne/Bloodreavers.cpp
        khorne/BloodWarriors.cpp
        khorne/Bloodletters.cpp
        khorne/Skullreapers.cpp
        khorne/Wrathmongers.cpp
        khorne/Khorgoraths.cpp
        khorne/BloodthirsterOfInsensateRage.cpp
        khorne/BloodthirsterOfUnfetteredFury.cpp
        khorne/WrathOfKhorneBloodthirster.cpp

        sylvaneth/Alarielle.cpp
        sylvaneth/SpiritOfDurthu.cpp
        sylvaneth/KurnothHunters.cpp
        sylvaneth/Dryads.cpp
        sylvaneth/TreeRevenants.cpp
        sylvaneth/SpiteRevenants.cpp

        gloomspitegitz/SquigHerd.cpp
        gloomspitegitz/SquigHoppers.cpp
        gloomspitegitz/BoingrotBounderz.cpp
        gloomspitegitz/LoonbossOnManglerSquigs.cpp
        gloomspitegitz/ManglerSquigs.cpp
        gloomspitegitz/ColossalSquig.cpp
        gloomspitegitz/Stabbas.cpp
        gloomspitegitz/Shootas.cpp
        gloomspitegitz/DankholdTroggboss.cpp
        gloomspitegitz/DankholdTroggoths.cpp
        gloomspitegitz/RockgutTroggoths.cpp

        dok/DaughterOfKhaine.cpp
        dok/WitchAelves.cpp
        dok/BloodSisters.cpp
        dok/KhineraiLifetakers.cpp
        dok/SistersOfSlaughter.cpp

        idonethdeepkin/NamartiThralls.cpp
        idonethdeepkin/NamartiReavers.cpp
        idonethdeepkin/IshlaenGuard.cpp
        idonethdeepkin/MorrsarrGuard.cpp

        nighthaunt/GrimghastReapers.cpp
        nighthaunt/ChainraspHorde.cpp
        nighthaunt/BladegheistRevenants.cpp
        nighthaunt/GlaivewraithStalkers.cpp
        nighthaunt/DreadscytheHarridans.cpp

        beastsofchaos/Ghorgon.cpp
        beastsofchaos/Gors.cpp
        beastsofchaos/Bestigors.cpp
        beastsofchaos/Bullgors.cpp
        beastsofchaos/Ungors.cpp

        slaanesh/Fiends.cpp
        slaanesh/Daemonettes.cpp
        slaanesh/Seekers.cpp

        nurgle/Plaguebearers.cpp
        nurgle/LordOfPlagues.cpp
        nurgle/PutridBlightkings.cpp

        death/BlackKnights.cpp
        death/BloodKnights.cpp
        death/DireWolves.cpp
        death/GraveGuard.cpp
        death/SkeletonWarriors.cpp
        death/Vargheists.cpp
        death/Zombies.cpp
)

target_include_directories(
        whSim PUBLIC
        ${GTK2_INCLUDE_DIRS}
        ../include
)

target_link_libraries(
        whSim
        ${GTK2_LIBRARIES}
)