/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <gloomspitegitz/Zarbag.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    Zarbag::Create,
    Zarbag::ValueToString,
    Zarbag::EnumStringToInt,
    {
        {ParamType::Enum, "Lore of the Moonclans", (int)LoreOfTheMoonclans::None, (int)LoreOfTheMoonclans::None, (int)LoreOfTheMoonclans::CallDaMoon, 1},
    },
    DESTRUCTION,
    { GLOOMSPITE_GITZ }
};

bool Zarbag::s_registered = false;

Zarbag::Zarbag() :
    GloomspiteGitzBase("Zarbag", 5, WOUNDS, 4, 6, false),
    m_sickle(Weapon::Type::Melee, "Cursed Sickle", 2, 3, 3, 3, -1, 1)
{
    m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, HERO, WIZARD, MADCAP_SHAMAN, ZARBAG};
    m_weapons = {&m_sickle};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool Zarbag::configure(LoreOfTheMoonclans lore)
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_sickle);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    //m_knownSpells.push_back(std::make_unique<FaceOfDaBadMoon>(this));
    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfTheMoonclans(lore, this)));

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Zarbag::Create(const ParameterList &parameters)
{
    auto unit = new Zarbag();
    auto lore = (LoreOfTheMoonclans)GetEnumParam("Lore of the Moonclans", parameters, (int)LoreOfTheMoonclans ::None);

    bool ok = unit->configure(lore);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Zarbag::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Zarbag", factoryMethod);
    }
}

std::string Zarbag::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Lore of the Moonclans")
    {
        return ToString((LoreOfTheMoonclans)parameter.intValue);
    }
    return ParameterValueToString(parameter);
}

int Zarbag::EnumStringToInt(const std::string &enumString)
{
    LoreOfTheMoonclans lore;
    if (FromString(enumString, lore))
    {
        return (int) lore;
    }
    return 0;
}

} // namespace GloomspiteGitz
