/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/MightyLordOfKhorne.h>
#include <UnitFactory.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    MightyLordOfKhorne::Create,
    KhorneBase::ValueToString,
    KhorneBase::EnumStringToInt,
    {
        {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
    },
    CHAOS,
    KHORNE
};

bool MightyLordOfKhorne::s_registered = false;

MightyLordOfKhorne::MightyLordOfKhorne() :
    KhorneBase("Mighty Lord of Khorne", 5, WOUNDS, 9, 3, false),
    m_axeOfKhorne(Weapon::Type::Melee, "Axe of Khorne", 1, 3, 3, 3, -1, RAND_D3),
    m_bloodDarkClaws(Weapon::Type::Melee, "Blood-dark Claws", 1, 4, 3, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, MIGHTY_LORD_OF_KHORNE};
    m_weapons = {&m_axeOfKhorne, &m_bloodDarkClaws};

    // Collar of Khorne
    m_totalUnbinds = 1;
}

bool MightyLordOfKhorne::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_axeOfKhorne);
    model->addMeleeWeapon(&m_bloodDarkClaws);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *MightyLordOfKhorne::Create(const ParameterList &parameters)
{
    auto unit = new MightyLordOfKhorne();

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

void MightyLordOfKhorne::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Mighty Lord of Khorne", factoryMethod);
    }
}


} // namespace Khorne