/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/TzeentchBase.h>
#include <Board.h>
#include "tzeentch/KairicAcolytes.h"
#include "tzeentch/HorrorsOfTzeentch.h"
#include "tzeentch/TzaangorEnlightened.h"
#include "tzeentch/TzaangorEnlightenedOnDisks.h"
#include "tzeentch/Tzaangors.h"
#include "tzeentch/FlamersOfTzeentch.h"
#include "tzeentch/ExaltedFlamersOfTzeentch.h"
#include "tzeentch/TzaangorSkyfires.h"
#include "tzeentch/TzaangorShaman.h"
#include "tzeentch/ScreamersOfTzeentch.h"
#include "tzeentch/LordOfChange.h"
#include "tzeentch/KairosFateweaver.h"
#include "tzeentch/MagisterOnDisc.h"
#include "tzeentch/Magister.h"
#include "tzeentch/GauntSummoner.h"
#include "tzeentch/OgroidThaumaturge.h"
#include "tzeentch/TheChangeling.h"
#include "tzeentch/Curseling.h"
#include "tzeentch/Fateskimmer.h"
#include "tzeentch/Vortemis.h"
#include "tzeentch/Fatemaster.h"

namespace Tzeentch
{

std::string TzeentchBase::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Change Coven")
    {
        if (parameter.intValue == None) return "None";
        else if (parameter.intValue == EternalConflagration) return "Eternal Conflagration";
        else if (parameter.intValue == HostsDuplicitous) return "Hosts Duplicitous";
        else if (parameter.intValue == HostsArcanum) return "Hosts Arcanum";
        else if (parameter.intValue == CultOfTheTransientForm) return "Cult of the Transient Form";
        else if (parameter.intValue == PyrofaneCult) return "Pyrofane Cult";
        else if (parameter.intValue == GuildOfSummoners) return "Guild of Summoners";
    }
    return ParameterValueToString(parameter);
}

int TzeentchBase::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "None") return None;
    else if (enumString == "Eternal Conflagration") return EternalConflagration;
    else if (enumString == "Hosts Duplicitous") return HostsDuplicitous;
    else if (enumString == "Hosts Arcanum") return HostsArcanum;
    else if (enumString == "Cult of the Transient Form") return CultOfTheTransientForm;
    else if (enumString == "Pyrofane Cult") return PyrofaneCult;
    else if (enumString == "Guild of Summoners") return GuildOfSummoners;

    return 0;
}

void TzeentchBase::setChangeCoven(ChangeCoven coven)
{
    removeKeyword(ETERNAL_CONFLAGRATION);
    removeKeyword(HOSTS_DUPLICITOUS);
    removeKeyword(HOSTS_ARCANUM);
    removeKeyword(CULT_OF_THE_TRANSIENT_FORM);
    removeKeyword(PYROFANE_CULT);
    removeKeyword(GUILD_OF_SUMMONERS);

    m_coven = coven;
    switch (coven)
    {
        case EternalConflagration:
            addKeyword(ETERNAL_CONFLAGRATION);
            break;
        case HostsDuplicitous:
            addKeyword(HOSTS_DUPLICITOUS);
            break;
        case HostsArcanum:
            addKeyword(HOSTS_ARCANUM);
            break;
        case CultOfTheTransientForm:
            addKeyword(CULT_OF_THE_TRANSIENT_FORM);
            break;
        case PyrofaneCult:
            addKeyword(PYROFANE_CULT);
            break;
        case GuildOfSummoners:
            addKeyword(GUILD_OF_SUMMONERS);
            break;
        default:
            break;
    }
}

int TzeentchBase::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Twiters of Materiality
    if (hasKeyword(ETERNAL_CONFLAGRATION) &&
        (weapon->name().find("Warpflame") || weapon->name().find("Magical Flames")))
    {
        return weapon->rend()-1;
    }
    return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
}

int TzeentchBase::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod =  Unit::toHitModifier(weapon, target);
    // Arrows of Tzeentch
    if (hasKeyword(PYROFANE_CULT) && hasKeyword(KAIRIC_ACOLYTES) && (weapon->name() == "Sorcerous Bolt"))
    {
        mod++;
    }
    return mod;
}

int TzeentchBase::targetHitModifier(const Weapon *weapon, const Unit *attacker) const
{
    auto mod = Unit::targetHitModifier(weapon, attacker);

    // Locus of Change
    if (!weapon->isMissile() && hasKeyword(DAEMON))
    {
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0f);
        for (auto unit : units)
        {
            if (unit->hasKeyword(HERO) && unit->hasKeyword(DAEMON))
            {
                mod--;
                break;
            }
        }
    }
    return mod;
}

void Init()
{
    CurselingEyeOfTzeentch::Init();
    ExaltedFlamersOfTzeentch::Init();
    Fatemaster::Init();
    Fateskimmer::Init();
    FlamersOfTzeentch::Init();
    GauntSummonerOfTzeentch::Init();
    HorrorsOfTzeentch::Init();
    KairicAcolytes::Init();
    KairosFateweaver::Init();
    LordOfChange::Init();
    Magister::Init();
    MagisterOnDiscOfTzeentch::Init();
    OgroidThaumaturge::Init();
    ScreamersOfTzeentch::Init();
    TheChangeling::Init();
    TzaangorEnlightened::Init();
    TzaangorEnlightenedOnDisks::Init();
    TzaangorShaman::Init();
    TzaangorSkyfires::Init();
    Tzaangors::Init();
    VortemisTheAllSeeing::Init();
}

} //namespace Tzeentch
