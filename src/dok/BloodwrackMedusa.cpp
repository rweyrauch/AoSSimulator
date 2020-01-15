/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/BloodwrackMedusa.h>
#include <UnitFactory.h>

namespace DaughtersOfKhaine
{
static FactoryMethod factoryMethod = {
    BloodwrackMedusa::Create,
    nullptr,
    nullptr,
    BloodwrackMedusa::ComputePoints,
    {
    },
    ORDER,
    { DAUGHTERS_OF_KHAINE }
};

bool BloodwrackMedusa::s_registered = false;

BloodwrackMedusa::BloodwrackMedusa() :
    DaughterOfKhaine("Bloodwrack Medusa", 8, WOUNDS, 8, 5, false),
    m_bloodwrackStare(Weapon::Type::Missile, "Bloodwrack Stare", 10, 0, 0, 0, 0, 0),
    m_whisperclaw(Weapon::Type::Melee, "Whisperclaw", 1, 4, 3, 3, 0, 1),
    m_tailOfSerpents(Weapon::Type::Melee, "Tail of Serpents", 2, RAND_D6, 4, 4, 0, 1),
    m_bloodwrackSpear(Weapon::Type::Melee, "Bloodwrack Spear", 2, 2, 3, 3, -1, RAND_D3)
{
    m_keywords = {ORDER, MELUSAI, DAUGHTERS_OF_KHAINE, HERO, WIZARD, BLOODWRACK_MEDUSA};
    m_weapons = {&m_bloodwrackStare, &m_whisperclaw, &m_tailOfSerpents, &m_bloodwrackSpear};
}

bool BloodwrackMedusa::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_bloodwrackStare);
    model->addMeleeWeapon(&m_whisperclaw);
    model->addMeleeWeapon(&m_tailOfSerpents);
    model->addMeleeWeapon(&m_bloodwrackSpear);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *BloodwrackMedusa::Create(const ParameterList &parameters)
{
    auto unit = new BloodwrackMedusa();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void BloodwrackMedusa::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bloodwrack Medusa", factoryMethod);
    }
}

} //namespace DaughtersOfKhaine

