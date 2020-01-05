/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/OrpheonKatakros.h"

namespace OssiarchBonereapers
{
static FactoryMethod factoryMethod = {
    OrpheonKatakros::Create,
    OrpheonKatakros::ValueToString,
    OrpheonKatakros::EnumStringToInt,
    {},
    DEATH,
    OSSIARCH_BONEREAPERS
};

bool OrpheonKatakros::s_registered = false;

Unit *OrpheonKatakros::Create(const ParameterList &parameters)
{
    auto unit = new OrpheonKatakros();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string OrpheonKatakros::ValueToString(const Parameter &parameter)
{
    return OssiarchBonereaperBase::ValueToString(parameter);
}

int OrpheonKatakros::EnumStringToInt(const std::string &enumString)
{
    return OssiarchBonereaperBase::EnumStringToInt(enumString);
}

void OrpheonKatakros::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Orpheon Katakros", factoryMethod);
    }
}

OrpheonKatakros::OrpheonKatakros() :
    OssiarchBonereaperBase("Orpheon Katakros", 4, WOUNDS, 10, 3, false),
    m_indaKhaat(Weapon::Type::Melee, "Inda-Khaat", 1, 1, 3, 3, -3, 3),
    m_shieldImmortis(Weapon::Type::Melee, "The Shield Immortis", 1, 4, 3, 3, -2, 2),
    m_nadiriteDagger(Weapon::Type::Melee, "Nadirite Dagger", 1, 1, 3, 3, -1, 1),
    m_blades(Weapon::Type::Melee, "Nadirite Duelling Blades", 1, 6, 3, 3, -1, 1),
    m_greatblade(Weapon::Type::Melee, "Soulreaver Greatblade", 1, 3, 3, 3, -1, 1),
    m_spiritDagger(Weapon::Type::Melee, "Spirit Dagger", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {DEATH, DEATHLORDS, OSSIARCH_BONEREAPERS, MORTIS_PRAETORIANS, LIEGE, HERO, KATAKROS};
    m_weapons = {&m_indaKhaat, &m_shieldImmortis, &m_nadiriteDagger, &m_blades, &m_greatblade, &m_spiritDagger};
}

bool OrpheonKatakros::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_indaKhaat);
    model->addMeleeWeapon(&m_shieldImmortis);
    model->addMeleeWeapon(&m_nadiriteDagger);
    model->addMeleeWeapon(&m_blades);
    model->addMeleeWeapon(&m_greatblade);
    model->addMeleeWeapon(&m_spiritDagger);

    m_shieldImmortis.activate(false);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

int OrpheonKatakros::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    if ((unmodifiedHitRoll == 6) && ((weapon->name() == m_nadiriteDagger.name()) || (weapon->name() == m_blades.name()))) return 2;
    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

void OrpheonKatakros::onWounded()
{
    Unit::onWounded();

    if (woundsTaken() >= 13)
    {
        m_shieldImmortis.activate(true);
        m_blades.activate(false);
        m_indaKhaat.setAttacks(4);
    }
    else if (woundsTaken() >= 8)
    {
        m_spiritDagger.activate(false);
    }
    else if (woundsTaken() >= 4)
    {
        m_greatblade.activate(false);
        m_indaKhaat.setAttacks(2);
    }
    else if (woundsTaken() >= 2)
    {
        m_nadiriteDagger.activate(false);
    }
}

Wounds OrpheonKatakros::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Deadly Combination
    if ((hitRoll == 6) && (weapon->name() == m_shieldImmortis.name()))
    {
        return {weapon->damage(), 2};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace OssiarchBonereapers
