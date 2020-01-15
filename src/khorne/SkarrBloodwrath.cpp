/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/SkarrBloodwrath.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    SkarrBloodwrath::Create,
    KhorneBase::ValueToString,
    KhorneBase::EnumStringToInt,
    SkarrBloodwrath::ComputePoints,
    {
        {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
    },
    CHAOS,
    { KHORNE }
};

bool SkarrBloodwrath::s_registered = false;

SkarrBloodwrath::SkarrBloodwrath() :
    KhorneBase("Skarr Bloodwrath", 5, WOUNDS, 9, 4, false),
    m_blades(Weapon::Type::Melee, "Bloodstorm Blades", 3, 5, 2, 3, -1, 1)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, SKARR_BLOODWRATH};
    m_weapons = {&m_blades};
}

bool SkarrBloodwrath::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_blades);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *SkarrBloodwrath::Create(const ParameterList &parameters)
{
    auto unit = new SkarrBloodwrath();

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

void SkarrBloodwrath::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Skarr Bloodwrath", factoryMethod);
    }
}

int SkarrBloodwrath::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    int attacks = KhorneBase::extraAttacks(attackingModel, weapon, target);
    // Slaughterstorm
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 3.0f);
    int totalModels = 0;
    for (auto ip : units)
    {
        // TODO: check for models within 3"
        totalModels += ip->remainingModels();
    }
    attacks += std::max(totalModels - m_blades.attacks(), 0);

    return attacks;
}

} // namespace Khorne