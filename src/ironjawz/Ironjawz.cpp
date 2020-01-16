/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/Ironjawz.h>

#include "ironjawz/OrrukArdboys.h"
#include "ironjawz/OrrukBrutes.h"
#include "ironjawz/GoreGruntas.h"
#include "ironjawz/OrrukMegaboss.h"
#include "ironjawz/OrrukWarchanter.h"
#include "ironjawz/Gordrakk.h"
#include "ironjawz/MegabossOnMawKrusha.h"
#include "ironjawz/IronskullsBoyz.h"

namespace Ironjawz
{

int Ironjawz::chargeModifier() const
{
    int mod = Unit::chargeModifier();

    // Eager for Battle
    mod++;

    // Hunt and Crush
    if (m_warclan == Bloodtoofs)
    {
        mod++;
    }
    return mod;
}

int Ironjawz::runModifier() const
{
    int mod = Unit::runModifier();

    // Hunt and Crush
    if (m_warclan == Bloodtoofs)
    {
        mod++;
    }
    return mod;
}

void Ironjawz::setWarclan(Ironjawz::Warclan warclan)
{
    removeKeyword(IRONSUNZ);
    removeKeyword(BLOODTOOFS);
    removeKeyword(CHOPPAS);

    m_warclan = warclan;
    switch (warclan)
    {
        case Ironsunz:
            addKeyword(IRONSUNZ);
            break;
        case Bloodtoofs:
            addKeyword(BLOODTOOFS);
            break;
        case DaChoppas:
            addKeyword(CHOPPAS);
            break;
        default:
            break;
    }
}

std::string Ironjawz::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Warclan")
    {
        if (parameter.intValue == Ironsunz) { return "Ironsunz"; }
        else if (parameter.intValue == Bloodtoofs) { return "Bloodtoofs"; }
        else if (parameter.intValue == DaChoppas) { return "Da Choppas"; }
    }
    return ParameterValueToString(parameter);
}

int Ironjawz::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Ironsunz") { return Ironsunz; }
    else if (enumString == "Bloodtoofs") { return Bloodtoofs; }
    else if (enumString == "Da Choppas") { return DaChoppas; }
    return 0;
}

void Init()
{
    OrrukArdboys::Init();
    OrrukBrutes::Init();
    OrrukGoreGruntas::Init();
    OrrukMegaboss::Init();
    OrrukWarchanter::Init();
    GordrakkTheFistOfGork::Init();
    MegabossOnMawKrusha::Init();
    IronskullsBoyz::Init();
}

} // namespace Ironjawz

