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
        stormcast/EvocatorsOnDracolines.cpp
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
        stormcast/VandusHammerhand.cpp
        stormcast/GryphHounds.cpp
        stormcast/Concussors.cpp
        stormcast/Desolators.cpp
        stormcast/Fulminators.cpp
        stormcast/Tempestors.cpp

        khorne/Bloodreavers.cpp
        khorne/BloodWarriors.cpp
        khorne/Bloodletters.cpp
        khorne/Skullreapers.cpp
        khorne/Wrathmongers.cpp
        khorne/Khorgoraths.cpp
        khorne/BloodthirsterOfInsensateRage.cpp
        khorne/BloodthirsterOfUnfetteredFury.cpp
        khorne/WrathOfKhorneBloodthirster.cpp
        khorne/Skarbrand.cpp
        khorne/Karanak.cpp
        khorne/FleshHounds.cpp

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
        gloomspitegitz/Skragrott.cpp
        gloomspitegitz/Loonboss.cpp
        gloomspitegitz/LoonbossOnGiantCaveSquig.cpp
        gloomspitegitz/LoonbossWithGiantCaveSquig.cpp
        gloomspitegitz/AleguzzlerGargant.cpp
        gloomspitegitz/SquigGobba.cpp

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

        fec/CryptHorrors.cpp
        fec/CryptGhouls.cpp
        fec/CryptFlayers.cpp
        fec/CryptInfernalCourtier.cpp

        wanderers/EternalGuard.cpp
        wanderers/GladeGuard.cpp
        wanderers/WildwoodRangers.cpp

        dispossessed/Hammerers.cpp
        dispossessed/Thunderers.cpp
        dispossessed/Warriors.cpp
        dispossessed/Longbeards.cpp
        dispossessed/Quarrellers.cpp
        dispossessed/Ironbreakers.cpp
        dispossessed/Irondrakes.cpp

        skaven/Clanrats.cpp
        skaven/GutterRunners.cpp
        skaven/NightRunners.cpp
        skaven/PlagueMonks.cpp
        skaven/Stormfiends.cpp
        skaven/Stormvermin.cpp
        skaven/HellPitAbomination.cpp
        skaven/RatlingGun.cpp
        skaven/WarpfireThrower.cpp
        skaven/WarpLightningCannon.cpp

        tzeentch/KairicAcolytes.cpp
        tzeentch/PinkHorrors.cpp
        tzeentch/TzaangorEnlightened.cpp
        tzeentch/Tzaangors.cpp
        tzeentch/FlamersOfTzeentch.cpp
)

target_include_directories(
        whSim PUBLIC
        ${GTK3_INCLUDE_DIRS}
        ../include
)

target_link_libraries(
        whSim
        ${GTK3_LIBRARIES}
)