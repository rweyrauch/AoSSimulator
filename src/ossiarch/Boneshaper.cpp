/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "ossiarch/Boneshaper.h"

namespace OssiarchBonereapers
{
static FactoryMethod factoryMethod = {
    MortisanBoneshaper::Create,
    MortisanBoneshaper::ValueToString,
    MortisanBoneshaper::EnumStringToInt,
    MortisanBoneshaper::ComputePoints,
    {
        {ParamType::Enum, "Legion", OssiarchBonereaperBase::None, OssiarchBonereaperBase::None, OssiarchBonereaperBase::Crematorians, 1},
    },
    DEATH,
    { OSSIARCH_BONEREAPERS }
};

bool MortisanBoneshaper::s_registered = false;

Unit *MortisanBoneshaper::Create(const ParameterList &parameters)
{
    auto unit = new MortisanBoneshaper();

    auto legion = (Legion)GetEnumParam("Legion", parameters, None);
    unit->setLegion(legion);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string MortisanBoneshaper::ValueToString(const Parameter &parameter)
{
    return OssiarchBonereaperBase::ValueToString(parameter);
}

int MortisanBoneshaper::EnumStringToInt(const std::string &enumString)
{
    return OssiarchBonereaperBase::EnumStringToInt(enumString);
}

void MortisanBoneshaper::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Mortisan Boneshaper", factoryMethod);
    }
}

MortisanBoneshaper::MortisanBoneshaper() :
    OssiarchBonereaperBase("Mortisan Boneshaper", 5, WOUNDS, 10, 4, false),
    m_talons(Weapon::Type::Melee, "Ossified Talons", 1, 2, 3, 4, 0, 1)
{
    m_keywords = {DEATH, OSSIARCH_BONEREAPERS, MORTISAN, HERO, WIZARD, MORTISAN_BONESHAPER};
    m_weapons = {&m_talons};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool MortisanBoneshaper::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_talons);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

} // namespace OssiarchBonereapers
