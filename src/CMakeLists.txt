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

add_library(
        AoSSim STATIC
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
        #jps.hh
        #pathfinders.cpp

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

        sylvaneth/SylvanethBase.cpp
        sylvaneth/Alarielle.cpp
        sylvaneth/SpiritOfDurthu.cpp
        sylvaneth/KurnothHunters.cpp
        sylvaneth/Dryads.cpp
        sylvaneth/TreeRevenants.cpp
        sylvaneth/SpiteRevenants.cpp
        sylvaneth/DrychaHamadreth.cpp
        sylvaneth/Treelord.cpp
        sylvaneth/TreelordAncient.cpp
        sylvaneth/Branchwraith.cpp
        sylvaneth/Branchwych.cpp
        sylvaneth/ArchRevenant.cpp
        sylvaneth/Ylthari.cpp
        sylvaneth/YlthariGardians.cpp
        sylvaneth/SkaethsWildHunt.cpp
        sylvaneth/SylvanethSpells.cpp

        dok/DaughterOfKhaine.cpp
        dok/WitchAelves.cpp
        dok/BloodSisters.cpp
        dok/KhineraiLifetakers.cpp
        dok/SistersOfSlaughter.cpp
        dok/KhineraiHeartrenders.cpp
        dok/DoomfireWarlocks.cpp
        dok/BloodStalkers.cpp
        dok/HagQueen.cpp
        dok/SlaughterQueen.cpp
        dok/AvatarOfKhaine.cpp
        dok/BloodwrackMedusa.cpp
        dok/BloodwrackShrine.cpp
        dok/HagQueenCauldronOfBlood.cpp
        dok/MorathiHighOracle.cpp
        dok/MorathiShadowQueen.cpp
        dok/SlaughterQueenCauldronOfBlood.cpp
        dok/TheBladeCoven.cpp
        dok/Morgwaeth.cpp
        dok/KhainiteShadowstalkers.cpp
        dok/MelusaiIronscale.cpp

        idonethdeepkin/IdonethDeepkin.cpp
        idonethdeepkin/NamartiThralls.cpp
        idonethdeepkin/NamartiReavers.cpp
        idonethdeepkin/IshlaenGuard.cpp
        idonethdeepkin/MorrsarrGuard.cpp
        idonethdeepkin/AkhelianLeviadon.cpp
        idonethdeepkin/AkhelianKing.cpp
        idonethdeepkin/Volturnos.cpp
        idonethdeepkin/AkhelianAllopexes.cpp
        idonethdeepkin/EidolonSea.cpp
        idonethdeepkin/EidolonStorm.cpp
        idonethdeepkin/Lotann.cpp
        idonethdeepkin/IsharannSoulscryer.cpp
        idonethdeepkin/IsharannSoulrender.cpp
        idonethdeepkin/IsharannTidecaster.cpp

        nighthaunt/Nighthaunt.cpp
        nighthaunt/GrimghastReapers.cpp
        nighthaunt/ChainraspHorde.cpp
        nighthaunt/BladegheistRevenants.cpp
        nighthaunt/GlaivewraithStalkers.cpp
        nighthaunt/DreadscytheHarridans.cpp
        nighthaunt/Hexwraiths.cpp
        nighthaunt/SpiritHosts.cpp
        nighthaunt/Chainghasts.cpp
        nighthaunt/GuardianOfSouls.cpp
        nighthaunt/KnightOfShrouds.cpp
        nighthaunt/KnightOfShroudsOnSteed.cpp
        nighthaunt/KurdossValentian.cpp
        nighthaunt/ReikenorGrimhailer.cpp
        nighthaunt/SpiritTorment.cpp
        nighthaunt/DreadbladeHarrow.cpp
        nighthaunt/LordExecutioner.cpp
        nighthaunt/TombBanshee.cpp
        nighthaunt/LadyOlynder.cpp
        nighthaunt/BlackCoach.cpp
        nighthaunt/CairnWraith.cpp
        nighthaunt/MyrmournBanshees.cpp
        nighthaunt/Mourngul.cpp

        slaanesh/SlaaneshBase.cpp
        slaanesh/Fiends.cpp
        slaanesh/Daemonettes.cpp
        slaanesh/Seekers.cpp
        slaanesh/KeeperOfSecrets.cpp
        slaanesh/Hellstriders.cpp
        slaanesh/ShalaxiHelbane.cpp
        slaanesh/SeekerChariots.cpp
        slaanesh/BladebringerSeekerChariot.cpp
        slaanesh/ExaltedChariot.cpp
        slaanesh/BladebringerExaltedChariot.cpp
        slaanesh/Hellflayer.cpp
        slaanesh/BladebringerHellflayer.cpp
        slaanesh/TheMasque.cpp
        slaanesh/Viceleader.cpp
        slaanesh/InfernalEnrapturess.cpp
        slaanesh/SyllEsske.cpp
        slaanesh/ContortedEpitome.cpp
        slaanesh/LordOfPain.cpp
        slaanesh/TheDreadPageant.cpp

        death/LegionOfNagash.cpp
        death/BlackKnights.cpp
        death/BloodKnights.cpp
        death/DireWolves.cpp
        death/GraveGuard.cpp
        death/SkeletonWarriors.cpp
        death/Vargheists.cpp
        death/Zombies.cpp
        death/WightKingBlackAxe.cpp
        death/WightKingTombBlade.cpp
        death/Necromancer.cpp
        death/Nagash.cpp
        death/BatSwarms.cpp
        death/BloodseekerPalanquin.cpp
        death/CorpseCartBrazier.cpp
        death/CorpseCartLodestone.cpp
        death/CovenThrone.cpp
        death/FellBats.cpp
        death/Mannfred.cpp
        death/MortisEngine.cpp
        death/Neferata.cpp
        death/PrinceVhordrai.cpp
        death/VampireLord.cpp
        death/VampireLordOnDragon.cpp

        fec/FleshEaterCourts.cpp
        fec/CryptHorrors.cpp
        fec/CryptGhouls.cpp
        fec/CryptFlayers.cpp
        fec/CryptInfernalCourtier.cpp
        fec/CryptGhastCourtier.cpp
        fec/CryptHaunterCourtier.cpp
        fec/AbhorrantArchregent.cpp
        fec/AbhorrantGhoulKing.cpp
        fec/GhoulKingOnTerrorgheist.cpp
        fec/GhoulKingOnZombieDragon.cpp
        fec/RoyalTerrorgheist.cpp
        fec/RoyalZombieDragon.cpp
        fec/VarghulfCourtier.cpp

        wanderers/Wanderer.cpp
        wanderers/EternalGuard.cpp
        wanderers/GladeGuard.cpp
        wanderers/WildwoodRangers.cpp

        dispossessed/Dispossessed.cpp
        dispossessed/Hammerers.cpp
        dispossessed/Thunderers.cpp
        dispossessed/Warriors.cpp
        dispossessed/Longbeards.cpp
        dispossessed/Quarrellers.cpp
        dispossessed/Ironbreakers.cpp
        dispossessed/Irondrakes.cpp
        dispossessed/Unforged.cpp
        dispossessed/Runelord.cpp
        dispossessed/WardenKing.cpp

        tzeentch/TzeentchBase.cpp
        tzeentch/KairicAcolytes.cpp
        tzeentch/HorrorsOfTzeentch.cpp
        tzeentch/TzaangorEnlightened.cpp
        tzeentch/TzaangorEnlightenedOnDisks.cpp
        tzeentch/Tzaangors.cpp
        tzeentch/FlamersOfTzeentch.cpp
        tzeentch/ExaltedFlamersOfTzeentch.cpp
        tzeentch/TzaangorSkyfires.cpp
        tzeentch/ScreamersOfTzeentch.cpp
        tzeentch/LordOfChange.cpp
        tzeentch/KairosFateweaver.cpp
        tzeentch/MagisterOnDisc.cpp
        tzeentch/Magister.cpp
        tzeentch/BurningChariots.cpp
        tzeentch/GauntSummoner.cpp
        tzeentch/TheChangeling.cpp
        tzeentch/OgroidThaumaturge.cpp
        tzeentch/TzaangorShaman.cpp
        tzeentch/Fateskimmer.cpp
        tzeentch/Curseling.cpp
        tzeentch/Vortemis.cpp
        tzeentch/Fatemaster.cpp
        tzeentch/BlueScribes.cpp
        tzeentch/ChangeCaster.cpp
        tzeentch/Fluxmaster.cpp
        tzeentch/TheEyesOfNine.cpp

        ironjawz/Ironjawz.cpp
        ironjawz/OrrukArdboys.cpp
        ironjawz/OrrukBrutes.cpp
        ironjawz/GoreGruntas.cpp
        ironjawz/OrrukMegaboss.cpp
        ironjawz/OrrukWarchanter.cpp
        ironjawz/Gordrakk.cpp
        ironjawz/MegabossOnMawKrusha.cpp
        ironjawz/IronskullsBoyz.cpp
        ironjawz/WeirdnobShaman.cpp
        ironjawz/MorgoksKrushas.cpp

        bonesplitterz/Bonesplitterz.cpp
        bonesplitterz/BoarboyManiaks.cpp
        bonesplitterz/ManiakWeirdnob.cpp
        bonesplitterz/OrrukArrowBoys.cpp
        bonesplitterz/OrrukMorboys.cpp
        bonesplitterz/SavageBigBoss.cpp
        bonesplitterz/SavageBigStabbas.cpp
        bonesplitterz/SavageBoarboys.cpp
        bonesplitterz/SavageOrruks.cpp
        bonesplitterz/Wardokk.cpp
        bonesplitterz/WurrgogProphet.cpp

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

        fyreslayers/AuricHearthguard.cpp
        fyreslayers/HearthguardBerzerkers.cpp
        fyreslayers/VulkiteBerzerkers.cpp
        fyreslayers/Doomseeker.cpp
        fyreslayers/Battlesmith.cpp
        fyreslayers/GrimwrathBerzerker.cpp
        fyreslayers/AuricRunefather.cpp
        fyreslayers/AuricRunemaster.cpp
        fyreslayers/AuricRuneson.cpp
        fyreslayers/AuricRunesmiter.cpp
        fyreslayers/AuricRunefatherOnMagmadroth.cpp
        fyreslayers/AuricRunesmiterOnMagmadroth.cpp
        fyreslayers/AuricRunesonOnMagmadroth.cpp
        fyreslayers/Fyreslayer.cpp

        everchosen/Archaon.cpp
        everchosen/Varanguard.cpp

        freeagents/GotrekGurnisson.cpp

        ossiarch/OssiarchBonereaperBase.cpp
        ossiarch/MortekGuard.cpp
        ossiarch/NecropolisStalkers.cpp
        ossiarch/Vokmortian.cpp
        ossiarch/KavalosDeathriders.cpp
        ossiarch/ImmortisGuard.cpp
        ossiarch/GothizzarHarvester.cpp
        ossiarch/MortekCrawler.cpp
        ossiarch/Katakros.cpp
        ossiarch/LiegeKavalos.cpp
        ossiarch/KavalosZandtos.cpp
        ossiarch/Boneshaper.cpp
        ossiarch/Soulmason.cpp
        ossiarch/Soulreaper.cpp
        ossiarch/MorghastArchai.cpp
        ossiarch/MorghastHarbingers.cpp
        ossiarch/ArkhanTheBlack.cpp

        kharadron/AethericNavigator.cpp
        kharadron/AetherKhemist.cpp
        kharadron/ArkanautAdmiral.cpp
        kharadron/ArkanautCompany.cpp
        kharadron/ArkanautFrigate.cpp
        kharadron/ArkanautIronclad.cpp
        kharadron/BrokkGrungsson.cpp
        kharadron/EndrinmasterWithHarness.cpp
        kharadron/EndrinmasterWithSuit.cpp
        kharadron/Endrinriggers.cpp
        kharadron/GrundstokGunhauler.cpp
        kharadron/GrundstokThunderers.cpp
        kharadron/KharadronBase.cpp
        kharadron/Skywardens.cpp
        kharadron/BjorgenThundrik.cpp
        kharadron/ThundriksProfiteers.cpp

        lumineth/LuminethRealmLords.cpp
        lumineth/LuminethPrivate.cpp
        lumineth/AuralanWardens.cpp
        lumineth/Dawnriders.cpp
        lumineth/Eltharion.cpp
        lumineth/Stoneguard.cpp
        lumineth/Stonemage.cpp
        lumineth/Cathallar.cpp
        lumineth/AuralanSentinels.cpp
        lumineth/SpiritOfTheMountain.cpp
        lumineth/Teclis.cpp
        lumineth/Avalenor.cpp
        lumineth/MyariLightcaller.cpp
        lumineth/MyarisPurifiers.cpp

        behemat/SonsOfBehemat.cpp
        behemat/Mancrusher.cpp
        behemat/Gatebreaker.cpp
        behemat/KrakenEater.cpp
        behemat/Warstomper.cpp
)

target_include_directories(
        AoSSim PUBLIC
        ${GTK3_INCLUDE_DIRS}
        ../include
)

target_link_libraries(
        AoSSim
        ${GTK3_LIBRARIES}
)

target_compile_options(
        AoSSim PRIVATE
        -Wpedantic
        -Wextra
        -Wall
        -Wformat-security
        -Wno-unused-parameter
)

target_compile_definitions(
        AoSSim PUBLIC HAVE_CAIRO
)