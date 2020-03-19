/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricHearthguard.h>
#include <UnitFactory.h>

namespace Fyreslayers
{

bool AuricHearthguard::s_registered = false;

AuricHearthguard::AuricHearthguard() :
    Fyreslayer("Auric Hearthguard", 4, WOUNDS, 7, 5, false),
    m_magmapike(Weapon::Type::Missile, "Magmapike", 18, 2, 4, 3, -1, 1),
    m_magmapikeKarl(Weapon::Type::Missile, "Magmapike", 18, 3, 4, 3, -1, 1),
    m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
    m_magmapikeMelee(Weapon::Type::Melee, "Magmapike", 1, 1, 3, 3, 0, 1)
{
    m_keywords = {ORDER, DUARDIN, FYRESLAYERS, AURIC_HEARTHGUARD};
    m_weapons = {&m_magmapike, &m_magmapikeKarl, &m_throwingAxe, &m_magmapikeMelee};
}

bool AuricHearthguard::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    auto karl = new Model(BASESIZE, WOUNDS);
    karl->addMissileWeapon(&m_magmapikeKarl);
    karl->addMissileWeapon(&m_throwingAxe);
    karl->addMeleeWeapon(&m_magmapikeMelee);
    addModel(karl);

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_magmapike);
        model->addMissileWeapon(&m_throwingAxe);
        model->addMeleeWeapon(&m_magmapikeMelee);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *AuricHearthguard::Create(const ParameterList &parameters)
{
    auto unit = new AuricHearthguard();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto lodge = (Lodge) GetEnumParam("Lodge", parameters, Fyreslayer::None);
    unit->setLodge(lodge);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void AuricHearthguard::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            AuricHearthguard::Create,
            Fyreslayer::ValueToString,
            Fyreslayer::EnumStringToInt,
            AuricHearthguard::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", AuricHearthguard::MIN_UNIT_SIZE, AuricHearthguard::MIN_UNIT_SIZE,
                    AuricHearthguard::MAX_UNIT_SIZE, AuricHearthguard::MIN_UNIT_SIZE
                },
                {ParamType::Enum, "Lodge", Fyreslayer::None, Fyreslayer::None, Fyreslayer::Lofnir, 1}
            },
            ORDER,
            { FYRESLAYERS }
        };
        s_registered = UnitFactory::Register("Auric Hearthguard", factoryMethod);
    }
}

Wounds AuricHearthguard::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Molten Rockbolts
    if ((weapon->name() == m_magmapike.name()) && target->hasKeyword(MONSTER))
    {
        return {weapon->damage()+1, 0};
    }
    return Fyreslayer::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int AuricHearthguard::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace Fyreslayers


