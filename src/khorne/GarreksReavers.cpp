/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/GarreksReavers.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne
{
static const int BASESIZE = 32;
static const int WOUNDS = 1;
static const int POINTS_PER_UNIT = 60;

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
        static FactoryMethod factoryMethod = {
            GarreksReavers::Create,
            KhorneBase::ValueToString,
            KhorneBase::EnumStringToInt,
            GarreksReavers::ComputePoints,
            {
                {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
            },
            CHAOS,
            { KHORNE }
        };
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
    m_weapons = {&m_garreksAxe, &m_KarusAxe, &m_SaeksAxe, &m_blades};
}

bool GarreksReavers::configure()
{
    auto garrek = new Model(BASESIZE, wounds());
    garrek->setName("Garrek");
    garrek->addMeleeWeapon(&m_garreksAxe);
    addModel(garrek);

    auto karsus = new Model(BASESIZE, wounds());
    karsus->setName("Karsus");
    karsus->addMeleeWeapon(&m_KarusAxe);
    addModel(karsus);

    auto saek = new Model(BASESIZE, wounds());
    saek->setName("Saek");
    saek->addMeleeWeapon(&m_SaeksAxe);
    addModel(saek);

    auto targor = new Model(BASESIZE, wounds());
    targor->setName("Targor");
    targor->addMeleeWeapon(&m_blades);
    addModel(targor);

    auto arnulf = new Model(BASESIZE, wounds());
    arnulf->setName("Arnulf");
    arnulf->addMeleeWeapon(&m_blades);
    addModel(arnulf);

    m_points = POINTS_PER_UNIT;

    return true;
}

Rerolls GarreksReavers::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Reaver Blades
    if (weapon->name() == m_blades.name())
    {
        return RerollOnes;
    }

    return KhorneBase::toHitRerolls(weapon, unit);
}

int GarreksReavers::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    int attacks = KhorneBase::extraAttacks(nullptr, weapon, target);

    // Frenzied Devotion
    auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 16.0f);
    for (auto ip : units)
    {
        if (ip->hasKeyword(KHORNE) && ip->hasKeyword(TOTEM))
        {
            attacks += 1;
            break;
        }
    }

    return attacks;
}

Wounds GarreksReavers::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    if ((hitRoll == 6) && (weapon->name() == m_garreksAxe.name()))
    {
        return { 0, 1};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int GarreksReavers::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} // namespace Khorne
