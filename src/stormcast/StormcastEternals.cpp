/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/StormcastEternals.h>
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

void Init()
{
    Liberators::Init();
    CelestarBallista::Init();
    Evocators::Init();
    Judicators::Init();
    Sequitors::Init();
    LordOrdinator::Init();
    Retributors::Init();
    LordCelestant::Init();
    LordCelestantOnDracoth::Init();
    LordCelestantOnStardrake::Init();
    KnightIncantor::Init();
    KnightQuestor::Init();
    KnightZephyros::Init();
    VanguardHunters::Init();
    Castigators::Init();
    VanguardRaptorsHurricane::Init();
    VanguardRaptorsLongstrike::Init();
    VandusHammerhand::Init();
    GryphHounds::Init();
    Concussors::Init();
    Desolators::Init();
    Fulminators::Init();
    Tempestors::Init();
    EvocatorsOnCelestialDracolines::Init();
    Prosecutors::Init();
    Protectors::Init();
    Decimators::Init();
    NeaveBlacktalon::Init();
    GavrielSureheart::Init();
    LordExorcist::Init();
    LordRelictor::Init();
    LordArcanumOnDracoline::Init();
    LordArcanumOnGryphcharger::Init();
    LordArcanumOnTauralon::Init();
    LordArcanum::Init();
    LordAquilor::Init();
    LordCastellant::Init();
    LordVeritant::Init();
}

} // namespace StormcastEternals