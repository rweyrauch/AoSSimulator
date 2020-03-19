/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/WebspinnerShaman.h>
#include <UnitFactory.h>
#include <iostream>
#include <spells/MysticShield.h>

namespace GloomspiteGitz
{

bool WebspinnerShaman::s_registered = false;

WebspinnerShaman::WebspinnerShaman() :
    GloomspiteGitzBase("Webspinner Shaman", 5, WOUNDS, 4, 6, false),
    m_spiderGodStaff(Weapon::Type::Melee, "Spider God Staff", 1, 1, 4, 3, -1, RAND_D3)
{
    m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, SPIDERFANG, HERO, WIZARD, WEBSPINNER_SHAMAN};
    m_weapons = {&m_spiderGodStaff};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool WebspinnerShaman::configure(LoreOfTheSpiderFangs lore)
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_spiderGodStaff);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    //m_knownSpells.push_back(std::make_unique<SpeedOfTheSpiderGod>(this));
    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfTheSpiderFangs(lore, this)));

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *WebspinnerShaman::Create(const ParameterList &parameters)
{
    auto unit = new WebspinnerShaman();
    auto lore = (LoreOfTheSpiderFangs)GetEnumParam("Lore of the Moonclans", parameters, (int)LoreOfTheSpiderFangs::None);

    bool ok = unit->configure(lore);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void WebspinnerShaman::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            WebspinnerShaman::Create,
            WebspinnerShaman::ValueToString,
            WebspinnerShaman::EnumStringToInt,
            WebspinnerShaman::ComputePoints,
            {
                {ParamType::Enum, "Lore of the Spiderfangs", (int)LoreOfTheSpiderFangs::None, (int)LoreOfTheSpiderFangs::None, (int)LoreOfTheSpiderFangs::GiftOfDaSpiderGod, 1},
            },
            DESTRUCTION,
            { GLOOMSPITE_GITZ }
        };
        s_registered = UnitFactory::Register("Webspinner Shaman", factoryMethod);
    }
}

std::string WebspinnerShaman::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Lore of the Spiderfangs")
    {
        return ToString((LoreOfTheSpiderFangs)parameter.intValue);
    }
    return ParameterValueToString(parameter);
}

int WebspinnerShaman::EnumStringToInt(const std::string &enumString)
{
    LoreOfTheSpiderFangs lore;
    if (FromString(enumString, lore))
    {
        return (int) lore;
    }
    return 0;
}

} // namespace GloomspiteGitz
