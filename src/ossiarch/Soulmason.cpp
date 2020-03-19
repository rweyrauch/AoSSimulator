/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "ossiarch/Soulmason.h"

namespace OssiarchBonereapers
{

bool MortisanSoulmason::s_registered = false;

Unit *MortisanSoulmason::Create(const ParameterList &parameters)
{
    auto unit = new MortisanSoulmason();

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

std::string MortisanSoulmason::ValueToString(const Parameter &parameter)
{
    return OssiarchBonereaperBase::ValueToString(parameter);
}

int MortisanSoulmason::EnumStringToInt(const std::string &enumString)
{
    return OssiarchBonereaperBase::EnumStringToInt(enumString);
}

void MortisanSoulmason::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            MortisanSoulmason::Create,
            MortisanSoulmason::ValueToString,
            MortisanSoulmason::EnumStringToInt,
            MortisanSoulmason::ComputePoints,
            {
                {ParamType::Enum, "Legion", OssiarchBonereaperBase::None, OssiarchBonereaperBase::None, OssiarchBonereaperBase::Crematorians, 1},
            },
            DEATH,
            { OSSIARCH_BONEREAPERS }
        };
        s_registered = UnitFactory::Register("Mortisan Soulmason", factoryMethod);
    }
}

MortisanSoulmason::MortisanSoulmason() :
    OssiarchBonereaperBase("Mortisan Soulmason", 5, WOUNDS, 10, 5, false),
    m_staff(Weapon::Type::Melee, "Soulmason's Staff", 2, 2, 4, 3, -1, RAND_D3),
    m_claws(Weapon::Type::Melee, "Ossified Claws", 1, 2, 4, 3, -1, 1)
{
    m_keywords = {DEATH, OSSIARCH_BONEREAPERS, MORTISAN, HERO, WIZARD, MORTISAN_SOULMASON};
    m_weapons = {&m_staff, &m_claws};

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

bool MortisanSoulmason::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_staff);
    model->addMeleeWeapon(&m_claws);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

} // namespace OssiarchBonereapers
