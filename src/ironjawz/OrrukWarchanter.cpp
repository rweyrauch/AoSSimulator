/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/OrrukWarchanter.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Ironjawz
{
static FactoryMethod factoryMethod = {
    OrrukWarchanter::Create,
    Ironjawz::ValueToString,
    Ironjawz::EnumStringToInt,
    {
        {ParamType::Enum, "Warclan", Ironjawz::Ironsunz, Ironjawz::Ironsunz, Ironjawz::DaChoppas, 1},
    },
    DESTRUCTION,
    IRONJAWZ
};

bool OrrukWarchanter::s_registered = false;

OrrukWarchanter::OrrukWarchanter() :
    Ironjawz("Orruk Warchanter", 4, WOUNDS, 7, 4, false),
    m_stikks(Weapon::Type::Melee, "Gorkstikk and Morkstikk", 1, 6, 4, 3, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, HERO, TOTEM, WARCHANTER};
}

bool OrrukWarchanter::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_stikks);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void OrrukWarchanter::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_stikks);
}

Unit *OrrukWarchanter::Create(const ParameterList &parameters)
{
    auto unit = new OrrukWarchanter();

    auto warclan = (Warclan)GetEnumParam("Warclan", parameters, Ironjawz::Ironsunz);
    unit->setWarclan(warclan);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void OrrukWarchanter::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Orruk Warchanter", factoryMethod);
    }
}

} // namespace Ironjawz
