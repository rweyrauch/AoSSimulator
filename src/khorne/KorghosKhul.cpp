/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/KorghosKhul.h>
#include <UnitFactory.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    KorghosKhul::Create,
    KhorneBase::ValueToString,
    KhorneBase::EnumStringToInt,
    {
        {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
    },
    CHAOS,
    KHORNE
};

bool KorghosKhul::s_registered = false;

KorghosKhul::KorghosKhul() :
    KhorneBase("Korghos Khul", 5, WOUNDS, 9, 3, false),
    m_axeOfKhorne(Weapon::Type::Melee, "Axe of Khorne", 1, 3, 3, 3, -1, RAND_D3),
    m_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 4, 3, 4, -1, 1)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, GORETIDE, HERO, MIGHTY_LORD_OF_KHORNE, KORGHOS_KHUL};

    // Collar of Khorne
    m_totalUnbinds = 1;
}

bool KorghosKhul::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_axeOfKhorne);
    model.addMeleeWeapon(&m_clawsAndFangs);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void KorghosKhul::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_axeOfKhorne);
    visitor(&m_clawsAndFangs);
}

Unit *KorghosKhul::Create(const ParameterList &parameters)
{
    auto unit = new KorghosKhul();

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

void KorghosKhul::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Korghos Khul", factoryMethod);
    }
}

Rerolls KorghosKhul::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Favoured of Khorne
    return RerollFailed;
}


} // namespace Khorne