/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Skullgrinder.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne
{

bool Skullgrinder::s_registered = false;

Skullgrinder::Skullgrinder() :
    KhorneBase("Skullgrinder", 5, WOUNDS, 8, 4, false),
    m_brazenAnvil(Weapon::Type::Melee, "Brazen Anvil", 2, 3, 3, 2, -1, 3)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, SKULLGRINDER};
    m_weapons = {&m_brazenAnvil};

    s_globalBraveryMod.connect(this, &Skullgrinder::favouredByKhorne, &m_connection);
}

Skullgrinder::~Skullgrinder()
{
    m_connection.disconnect();
}

bool Skullgrinder::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMeleeWeapon(&m_brazenAnvil);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Skullgrinder::Create(const ParameterList &parameters)
{
    auto unit = new Skullgrinder();

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

void Skullgrinder::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            ValueToString,
            EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
            },
            CHAOS,
            { KHORNE }
        };
        s_registered = UnitFactory::Register("Skullgrinder", factoryMethod);
    }
}

std::string Skullgrinder::ValueToString(const Parameter &parameter)
{
    return KhorneBase::ValueToString(parameter);
}

int Skullgrinder::EnumStringToInt(const std::string &enumString)
{
    return KhorneBase::EnumStringToInt(enumString);
}

int Skullgrinder::favouredByKhorne(const Unit *unit)
{
    // Favoured by Khorne
    if (unit->hasKeyword(KHORNE) && unit->hasKeyword(MORTAL) &&
        (unit->owningPlayer() == owningPlayer()) && (distanceTo(unit) <= 12.0f))
    {
        return 1;
    }
    return 0;
}

Wounds Skullgrinder::onEndCombat(PlayerId player)
{
    auto wounds = Unit::onEndCombat(player);

    // Fiery Anvil
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 2.0f);
    for (auto unit : units)
    {
        if (unit->hasKeyword(HERO) || unit->hasKeyword(MONSTER))
        {
            if (Dice::rollD6() >= 2)
            {
                Wounds anvilWounds = {0, Dice::rollD3()};
                unit->applyDamage(anvilWounds);
                wounds += anvilWounds;
                break;
            }
        }
    }
    return wounds;
}


} // namespace Khorne