/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <stormcast/StormcastEternals.h>
#include <Board.h>
#include <Roster.h>
#include "stormcast/Liberators.h"
#include "stormcast/CelestarBallista.h"
#include "stormcast/Evocators.h"
#include "stormcast/Judicators.h"
#include "stormcast/Sequitors.h"
#include "stormcast/LordOrdinator.h"
#include "stormcast/Retributors.h"
#include "stormcast/LordCelestant.h"
#include "stormcast/LordCelestantOnDracoth.h"
#include "stormcast/LordCelestantOnStardrake.h"
#include "stormcast/KnightIncantor.h"
#include "stormcast/KnightQuestor.h"
#include "stormcast/KnightZephyros.h"
#include "stormcast/VanguardHunters.h"
#include "stormcast/VanguardRaptorsHurricane.h"
#include "stormcast/VanguardRaptorsLongstrike.h"
#include "stormcast/Castigators.h"
#include "stormcast/VandusHammerhand.h"
#include "stormcast/GryphHounds.h"
#include "stormcast/Concussors.h"
#include "stormcast/Desolators.h"
#include "stormcast/Fulminators.h"
#include "stormcast/Tempestors.h"
#include "stormcast/EvocatorsOnDracolines.h"
#include "stormcast/Prosecutors.h"
#include "stormcast/Protectors.h"
#include "stormcast/Decimators.h"
#include "stormcast/NeaveBlacktalon.h"
#include "stormcast/GavrielSureheart.h"
#include "stormcast/LordExorcist.h"
#include "stormcast/LordRelictor.h"
#include "stormcast/LordArcanumOnDracoline.h"
#include "stormcast/LordArcanumOnGryphcharger.h"
#include "stormcast/LordArcanumOnTauralon.h"
#include "stormcast/LordArcanum.h"
#include "stormcast/LordAquilor.h"
#include "stormcast/LordCastellant.h"
#include "stormcast/LordVeritant.h"
#include "stormcast/KnightAzyros.h"
#include "stormcast/KnightHeraldor.h"
#include "stormcast/KnightVenator.h"
#include "stormcast/KnightVexillor.h"
#include "stormcast/AventisFirestrike.h"
#include "stormcast/AstreiaSolbright.h"
#include "stormcast/VanguardPalladors.h"
#include "stormcast/DrakeswornTemplar.h"
#include "stormcast/CelestantPrime.h"
#include "stormcast/Aetherwings.h"
#include "stormcast/SteelheartsChampions.h"
#include "stormcast/TheFarstriders.h"
#include "stormcast/StormsiresCursebreakers.h"
#include "stormcast/AveronStormsire.h"

namespace StormcastEternals
{

int StormcastEternal::toHitModifier(const Weapon *weapon, const Unit *unit) const
{
    int modifier = Unit::toHitModifier(weapon, unit);

    // Beast Stalkers
    if (hasKeyword(ASTRAL_TEMPLARS) && unit->hasKeyword(MONSTER))
    {
        modifier += 1;
    }

    return modifier;
}

Rerolls StormcastEternal::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Driven by Vengeance
    if (hasKeyword(CELESTIAL_VINDICATORS) && m_charged)
    {
        return RerollOnes;
    }

    return Unit::toHitRerolls(weapon, unit);
}

int StormcastEternal::braveryModifier() const
{
    int modifier = Unit::braveryModifier();

    // First to be Forged
    if (hasKeyword(HAMMERS_OF_SIGMAR))
    {
        modifier += 1;
    }

    return modifier;
}

Rerolls StormcastEternal::battleshockRerolls() const
{
    // No True Death
    if (hasKeyword(ANVILS_OF_THE_HELDENHAMMER))
    {
        return RerollFailed;
    }

    return Unit::battleshockRerolls();
}

void StormcastEternal::setStormhost(Stormhost host)
{
    removeKeyword(HAMMERS_OF_SIGMAR);
    removeKeyword(HALLOWED_KNIGHTS);
    removeKeyword(CELESTIAL_VINDICATORS);
    removeKeyword(ANVILS_OF_THE_HELDENHAMMER);
    removeKeyword(KNIGHTS_EXCELSIOR);
    removeKeyword(CELESTIAL_WARBRINGERS);
    removeKeyword(TEMPEST_LORDS);
    removeKeyword(ASTRAL_TEMPLARS);

    m_stormHost = host;
    switch (m_stormHost)
    {
        case HammersOfSigmar:
            addKeyword(HAMMERS_OF_SIGMAR);
            break;
        case HallowedKnights:
            addKeyword(HALLOWED_KNIGHTS);
            break;
        case CelestialVindicators:
            addKeyword(CELESTIAL_VINDICATORS);
            break;
        case AnvilsOfTheHeldenhammer:
            addKeyword(ANVILS_OF_THE_HELDENHAMMER);
            break;
        case KnightsExcelsior:
            addKeyword(KNIGHTS_EXCELSIOR);
            break;
        case CelestialWarbringers:
            addKeyword(CELESTIAL_WARBRINGERS);
            break;
        case TempestLords:
            addKeyword(TEMPEST_LORDS);
            break;
        case AstralTemplars:
            addKeyword(ASTRAL_TEMPLARS);
            break;
        default:
            break;
    }
}

std::string StormcastEternal::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Stormhost")
    {
        if (parameter.intValue == HammersOfSigmar) { return "Hammers of Sigmar"; }
        else if (parameter.intValue == HallowedKnights) { return "Hallowed Knights"; }
        else if (parameter.intValue == CelestialVindicators) { return "Celestial Vindicators"; }
        else if (parameter.intValue == AnvilsOfTheHeldenhammer) { return "Anvils of the Heldenhammer"; }
        else if (parameter.intValue == KnightsExcelsior) return "Knights Excelsior";
        else if (parameter.intValue == CelestialWarbringers) return "Celestial Warbringers";
        else if (parameter.intValue == TempestLords) return "Tempest Lords";
        else if (parameter.intValue == AstralTemplars) return "Astral Templars";
        else if (parameter.intValue == None) { return "None"; }
    }
    return ParameterValueToString(parameter);
}

int StormcastEternal::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Hammers of Sigmar") { return HammersOfSigmar; }
    else if (enumString == "Hallowed Knights") { return HallowedKnights; }
    else if (enumString == "Celestial Vindicators") { return CelestialVindicators; }
    else if (enumString == "Anvils of the Heldenhammer") { return AnvilsOfTheHeldenhammer; }
    else if (enumString == "Knights Excelsior") { return KnightsExcelsior; }
    else if (enumString == "Celestial Warbringers") { return CelestialWarbringers; }
    else if (enumString == "Tempest Lords") { return TempestLords; }
    else if (enumString == "Astral Templars") { return AstralTemplars; }
    else if (enumString == "None") { return None; }
    return 0;
}

void StormcastEternal::onStartHero(PlayerId player)
{
    Unit::onStartHero(player);

    // Grand Strategists
    if ((owningPlayer() == player) && hasKeyword(TEMPEST_LORDS))
    {
        if (Dice::rollD6() >= 4)
        {
            m_roster->addCommandPoints(1);
        }
    }
}

void Init()
{
    CelestantPrime::Init();
    AventisFirestrike::Init();
    AstreiaSolbright::Init();
    VandusHammerhand::Init();
    NeaveBlacktalon::Init();
    GavrielSureheart::Init();
    SteelheartsChampions::Init();
    TheFarstriders::Init();
    AveronStormsire::Init();
    StormsiresCursebreakers::Init();
    LordArcanumOnDracoline::Init();
    LordArcanumOnGryphcharger::Init();
    LordArcanumOnTauralon::Init();
    LordArcanum::Init();
    LordAquilor::Init();
    LordCelestant::Init();
    LordCelestantOnDracoth::Init();
    LordCelestantOnStardrake::Init();
    LordCastellant::Init();
    LordOrdinator::Init();
    LordExorcist::Init();
    LordRelictor::Init();
    LordVeritant::Init();
    KnightAzyros::Init();
    KnightHeraldor::Init();
    KnightQuestor::Init();
    KnightIncantor::Init();
    KnightVenator::Init();
    KnightVexillor::Init();
    KnightZephyros::Init();
    GryphHounds::Init();
    DrakeswornTemplar::Init();
    Concussors::Init();
    Desolators::Init();
    Fulminators::Init();
    Tempestors::Init();
    Prosecutors::Init();
    Decimators::Init();
    Protectors::Init();
    Retributors::Init();
    Liberators::Init();
    Judicators::Init();
    VanguardRaptorsHurricane::Init();
    VanguardRaptorsLongstrike::Init();
    VanguardHunters::Init();
    VanguardPalladors::Init();
    Aetherwings::Init();
    Castigators::Init();
    CelestarBallista::Init();
    Sequitors::Init();
    Evocators::Init();
    EvocatorsOnCelestialDracolines::Init();
}

bool DoSpiritFlasks(Unit* owner)
{
    auto units = Board::Instance()->getUnitsWithin(owner, GetEnemyId(owner->owningPlayer()), 3.0f);
    int potentialWoundsPerFlask = 0;
    for (auto ip : units)
    {
        if (ip->remainingModels() >= 10)
            potentialWoundsPerFlask += 3;
        else
            potentialWoundsPerFlask++;
    }

    int numFlasks = 0;
    if (potentialWoundsPerFlask > owner->remainingWounds() * 6)
    {
        numFlasks = 3;
    }
    else if (potentialWoundsPerFlask > owner->remainingWounds() * 4)
    {
        numFlasks = 2;
    }
    else if (potentialWoundsPerFlask > owner->remainingWounds() * 2)
    {
        numFlasks = 1;
    }

    // Going out with a bang!
    if (owner->remainingWounds() == 1)
    {
        numFlasks = 3;
    }

    if (numFlasks)
    {
        for (auto ip : units)
        {
            int damage = 1;
            if (ip->remainingModels() >= 10) damage = RAND_D3;

            int mortalsTarget = 0;
            for (auto i = 0; i < numFlasks; i++)
            {
                mortalsTarget += Dice::rollSpecial(damage);
            }

            auto numSlain = ip->applyDamage({0, mortalsTarget});

            SimLog(Verbosity::Narrative, "%s shattered %d Spirit Flasks inflicting %d mortal wounds on %s slaying %d.\n",
                   owner->name().c_str(), numFlasks, mortalsTarget, ip->name().c_str(), numSlain);
        }

        int mortalsSelf = numFlasks;
        auto dead = owner->applyDamage({0, mortalsSelf});
        SimLog(Verbosity::Narrative, "Spirit Flasks inflicted %s wounds on %s.  Slaying %d models.\n",
               mortalsSelf, owner->name().c_str(), dead);
    }

    return (numFlasks != 0);
}

} // namespace StormcastEternals
