/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <bonesplitterz/Bonesplitterz.h>

#include "bonesplitterz/BoarboyManiaks.h"
#include "bonesplitterz/ManiakWeirdnob.h"
#include "bonesplitterz/OrrukArrowBoys.h"
#include "bonesplitterz/OrrukMorboys.h"
#include "bonesplitterz/SavageBigStabbas.h"
#include "bonesplitterz/SavageBigBoss.h"
#include "bonesplitterz/SavageBoarboys.h"
#include "bonesplitterz/SavageOrruks.h"
#include "bonesplitterz/Wardokk.h"
#include "bonesplitterz/WurrgogProphet.h"

namespace Bonesplitterz
{


void Bonesplitterz::setWarclan(Warclan warclan)
{
    m_warclan = warclan;
    switch (warclan)
    {
        case Bonegrinz:
            addKeyword(BONEGRINZ);
            break;
        case Drakkfoot:
            addKeyword(DRAKKFOOT);
            break;
        case Icebone:
            addKeyword(ICEBONE);
            break;
        default:
            break;
    }
}

std::string Bonesplitterz::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Warclan")
    {
        if (parameter.m_intValue == Bonegrinz) { return "Bonegrinz"; }
        else if (parameter.m_intValue == Drakkfoot) { return "Drakkfoot"; }
        else if (parameter.m_intValue == Icebone) { return "Icebone"; }
    }
    return ParameterValueToString(parameter);
}

int Bonesplitterz::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Bonegrinz") { return Bonegrinz; }
    else if (enumString == "Drakkfoot") { return Drakkfoot; }
    else if (enumString == "Icebone") { return Icebone; }
    return 0;
}

void Init()
{
    SavageBoarboyManiaks::Init();
    ManiakWeirdnob::Init();
    SavageOrrukArrowboys::Init();
    SavageOrrukMorboys::Init();
    SavageBigBoss::Init();
    SavageBigStabbas::Init();
    SavageBoarboys::Init();
    SavageOrruks::Init();
    Wardokk::Init();
    WurrgogProphet::Init();
}

} // namespace Bonesplitterz
