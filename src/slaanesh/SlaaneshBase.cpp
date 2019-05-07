/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/SlaaneshBase.h>
#include "slaanesh/Fiends.h"
#include "slaanesh/Seekers.h"
#include "slaanesh/Daemonettes.h"
#include "slaanesh/KeeperOfSecrets.h"
#include "slaanesh/Hellstriders.h"
#include "slaanesh/ShalaxiHelbane.h"
#include "slaanesh/BladebringerExaltedChariot.h"
#include "slaanesh/BladebringerHellflayer.h"
#include "slaanesh/BladebringerSeekerChariot.h"
#include "slaanesh/ExaltedChariot.h"
#include "slaanesh/Hellflayer.h"
#include "slaanesh/SeekerChariots.h"
#include "slaanesh/TheMasque.h"
#include "slaanesh/Viceleader.h"
#include "slaanesh/InfernalEnrapturess.h"
#include "slaanesh/SyllEsske.h"
#include "slaanesh/ContortedEpitome.h"

namespace Slaanesh
{

void SlaaneshBase::setHost(Host host)
{
    m_host = host;
    switch (m_host)
    {
        case Invaders:
            addKeyword(INVADERS);
            break;
        case Pretenders:
            addKeyword(PRETENDERS);
            break;
        case Godseekers:
            addKeyword(GODSEEKERS);
            break;
        default:
            break;
    }
}

std::string SlaaneshBase::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Host")
    {
        if (parameter.m_intValue == Invaders) { return "Invaders"; }
        else if (parameter.m_intValue == Pretenders) { return "Pretenders"; }
        else if (parameter.m_intValue == Godseekers) { return "Godseekers"; }
    }
    return ParameterValueToString(parameter);
}

int SlaaneshBase::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Invaders") { return Invaders; }
    else if (enumString == "Pretenders") { return Pretenders; }
    else if (enumString == "Godseekers") { return Godseekers; }
    return 0;
}

int SlaaneshBase::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    int hits = Unit::generateHits(unmodifiedHitRoll, weapon, unit);

    // Euphoric Killers
    if ((unmodifiedHitRoll == 6) && !weapon->isMissile())
    {
        hits += 1;
        if (remainingModels() >= 20)
        {
            hits += 1;
        }
    }
    return hits;
}

Rerolls SlaaneshBase::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Heir to the Throne
    if ((m_host == Pretenders) && (remainingModels() >= 10))
    {
        return RerollOnes;
    }
    return Unit::toHitRerolls(weapon, target);
}

int SlaaneshBase::chargeModifier() const
{
    int modifier = Unit::chargeModifier();

    // Thundering Cavalcade
    if (m_host == Godseekers)
    {
        modifier += 1;
    }
    return modifier;
}

void Init()
{
    Daemonettes::Init();
    Fiends::Init();
    Seekers::Init();
    KeeperOfSecrets::Init();
    Hellstriders::Init();
    ShalaxiHelbane::Init();
    BladebringerOnExaltedChariot::Init();
    BladebringerOnHellflayer::Init();
    BladebringerOnSeekerChariot::Init();
    ExaltedChariot::Init();
    Hellflayer::Init();
    SeekerChariots::Init();
    TheMasque::Init();
    ViceleaderHeraldOfSlaanesh::Init();
    InfernalEnrapturess::Init();
    SyllEsske::Init();
    TheContortedEpitome::Init();
}

} // namespace Slaanesh
