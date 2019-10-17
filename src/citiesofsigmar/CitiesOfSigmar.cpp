/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/CitiesOfSigmar.h"

#include "citiesofsigmar/Anointed.h"
#include "citiesofsigmar/Assassin.h"
#include "citiesofsigmar/Battlemage.h"
#include "citiesofsigmar/BattlemageOnGriffon.h"
#include "citiesofsigmar/BlackArkCorsairs.h"
#include "citiesofsigmar/BlackArkFleetmaster.h"
#include "citiesofsigmar/BlackGuard.h"
#include "citiesofsigmar/Bleakswords.h"
#include "citiesofsigmar/CelestialHurricanum.h"
#include "citiesofsigmar/Cogsmith.h"
#include "citiesofsigmar/DarkRiders.h"
#include "citiesofsigmar/Darkshards.h"
#include "citiesofsigmar/DemigryphKnights.h"
#include "citiesofsigmar/DrakespawnChariots.h"
#include "citiesofsigmar/DrakespawnKnights.h"
#include "citiesofsigmar/DreadlordOnBlackDragon.h"
#include "citiesofsigmar/Dreadspears.h"
#include "citiesofsigmar/EternalGuard.h"
#include "citiesofsigmar/Executioners.h"
#include "citiesofsigmar/Flagellants.h"
#include "citiesofsigmar/FlamespyrePhoenix.h"
#include "citiesofsigmar/FreeguildCrossbowmen.h"
#include "citiesofsigmar/FreeguildGeneral.h"
#include "citiesofsigmar/FreeguildGeneralOnGriffon.h"
#include "citiesofsigmar/FreeguildGreatswords.h"
#include "citiesofsigmar/FreeguildGuard.h"
#include "citiesofsigmar/FreeguildHandgunners.h"
#include "citiesofsigmar/FreeguildOutriders.h"
#include "citiesofsigmar/FreeguildPistoliers.h"
#include "citiesofsigmar/FrostheartPhoenix.h"
#include "citiesofsigmar/Gyrobombers.h"
#include "citiesofsigmar/Gyrocopters.h"
#include "citiesofsigmar/Hammerers.h"
#include "citiesofsigmar/HelblasterVolleyGun.h"
#include "citiesofsigmar/HelstormRocketBattery.h"
#include "citiesofsigmar/Ironbreakers.h"
#include "citiesofsigmar/Irondrakes.h"
#include "citiesofsigmar/Kharibdyss.h"
#include "citiesofsigmar/Longbeards.h"
#include "citiesofsigmar/LuminarkOfHysh.h"
#include "citiesofsigmar/NomadPrince.h"
#include "citiesofsigmar/PhoenixGuard.h"
#include "citiesofsigmar/Runelord.h"
#include "citiesofsigmar/ScourgerunnerChariots.h"
#include "citiesofsigmar/ShadowWarriors.h"
#include "citiesofsigmar/SistersOfTheThorn.h"
#include "citiesofsigmar/SistersOfTheWatch.h"
#include "citiesofsigmar/Sorceress.h"
#include "citiesofsigmar/SorceressOnBlackDragon.h"
#include "citiesofsigmar/SteamTank.h"
#include "citiesofsigmar/WardenKing.h"
#include "citiesofsigmar/WarHydra.h"
#include "citiesofsigmar/WildRiders.h"
#include "citiesofsigmar/WildwoodRangers.h"


namespace CitiesOfSigmar
{

std::string CitizenOfSigmar::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int CitizenOfSigmar::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void CitizenOfSigmar::setCity(CitizenOfSigmar::City city)
{

}

void Init()
{
    Anointed::Init();
    Assassin::Init();
    Battlemage::Init();
    BattlemageOnGriffon::Init();
    BlackArkCorsairs::Init();
    BlackArkFleetmaster::Init();
    BlackGuard::Init();
    Bleakswords::Init();
    CelestialHurricanum::Init();
    Cogsmith::Init();
    DarkRiders::Init();
    Darkshards::Init();
    DemigryphKnights::Init();
    DrakespawnChariots::Init();
    DrakespawnKnights::Init();
    DreadlordOnBlackDragon::Init();
    Dreadspears::Init();
    EternalGuard::Init();
    Executioners::Init();
    Flagellants::Init();
    FlamespyrePhoenix::Init();
    FreeguildCrossbowmen::Init();
    FreeguildGeneral::Init();
    FreeguildGeneralOnGriffon::Init();
    FreeguildGreatswords::Init();
    FreeguildGuard::Init();
    FreeguildHandgunners::Init();
    FreeguildOutriders::Init();
    FreeguildPistoliers::Init();
    FrostheartPhoenix::Init();
    Gyrobombers::Init();
    Gyrocopters::Init();
    Hammerers::Init();
    HelblasterVolleyGun::Init();
    HelstormRocketBattery::Init();
    Ironbreakers::Init();
    Irondrakes::Init();
    Kharibdyss::Init();
    Longbeards::Init();
    LuminarkOfHysh::Init();
    NomadPrince::Init();
    PhoenixGuard::Init();
    Runelord::Init();
    ScourgerunnerChariots::Init();
    ShadowWarriors::Init();
    SistersOfTheThorn::Init();
    SistersOfTheWatch::Init();
    Sorceress::Init();
    SorceressOnBlackDragon::Init();
    SteamTank::Init();
    WardenKing::Init();
    WarHydra::Init();
    WildRiders::Init();
    WildwoodRangers::Init();
}

} // namespace CitiesOfSigmar