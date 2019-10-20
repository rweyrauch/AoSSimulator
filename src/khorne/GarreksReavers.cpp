/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/GarreksReavers.h>
#include <UnitFactory.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    GarreksReavers::Create,
    KhorneBase::ValueToString,
    KhorneBase::EnumStringToInt,
    {
        {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
    },
    CHAOS,
    KHORNE
};

bool GarreksReavers::s_registered = false;

Unit *GarreksReavers::Create(const ParameterList &parameters)
{
    auto unit = new GarreksReavers();

    auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, KhorneBase::None);
    unit->setSlaughterHost(host);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void GarreksReavers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Garrek's Reavers", factoryMethod);
    }
}

GarreksReavers::GarreksReavers() :
    KhorneBase("Garrek's Reavers", 6, WOUNDS, 6, 6, false),
    m_garreksAxe(Weapon::Type::Melee, "Garrek's Blooddrinker Axe", 1, 3, 3, 4, -1, 1),
    m_KarusAxe(Weapon::Type::Melee, "Karsus' Chained Axe", 2, RAND_D3, 4, 4, 0, 1),
    m_SaeksAxe(Weapon::Type::Melee, "Saek's Meatripper Axe", 1, 1, 3, 4, -1, 1),
    m_blades(Weapon::Type::Melee, "Reaver Blades", 1, 1, 3, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, GORETIDE, BLOODREAVERS, GARREKS_REAVERS};
}

bool GarreksReavers::configure()
{
    Model garrek(BASESIZE, WOUNDS);
    garrek.setName("Garrek");
    garrek.addMeleeWeapon(&m_garreksAxe);
    addModel(garrek);

    Model karsus(BASESIZE, WOUNDS);
    karsus.setName("Karsus");
    karsus.addMeleeWeapon(&m_KarusAxe);
    addModel(karsus);

    Model saek(BASESIZE, WOUNDS);
    saek.setName("Saek");
    saek.addMeleeWeapon(&m_SaeksAxe);
    addModel(saek);

    Model reaver(BASESIZE, WOUNDS);
    reaver.setName("Targor");
    reaver.addMeleeWeapon(&m_blades);
    addModel(reaver);

    reaver.setName("Arnulf");
    addModel(reaver);

    m_points = POINTS_PER_UNIT;

    return true;
}

void GarreksReavers::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_garreksAxe);
    visitor(m_KarusAxe);
    visitor(m_SaeksAxe);
    visitor(m_blades);
}

} // namespace Khorne
