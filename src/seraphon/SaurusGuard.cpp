/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusGuard.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    SaurusGuard::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", SaurusGuard::MIN_UNIT_SIZE, SaurusGuard::MIN_UNIT_SIZE,
            SaurusGuard::MAX_UNIT_SIZE, SaurusGuard::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Stardrake Icon", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Wardrum", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0}
    },
    ORDER,
    SERAPHON
};

bool SaurusGuard::s_registered = false;

SaurusGuard::SaurusGuard() :
    Unit("Saurus Guard", 5, WOUNDS, 10, 4, false),
    m_celestitePolearm(Weapon::Type::Melee, "Celestite Polearm", 1, 2, 3, 3, -1, 1),
    m_celestitePolearmAlpha(Weapon::Type::Melee, "Celestite Polearm", 1, 3, 3, 3, -1, 1),
    m_jawsAndShield(Weapon::Type::Melee, "Powerful Jaws and Stardrake Shield", 1, 1, 5, 4, 0, 1)
{
    m_keywords = {ORDER, DAEMON, CELESTIAL, SERAPHON, SAURUS, SAURUS_GUARD};
}

bool SaurusGuard::configure(int numModels, bool iconBearer, bool wardrum)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    m_wardrum = wardrum;

    // Add the Alpha
    Model alpha(BASESIZE, WOUNDS);
    alpha.addMeleeWeapon(&m_celestitePolearmAlpha);
    alpha.addMeleeWeapon(&m_jawsAndShield);
    addModel(alpha);

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_celestitePolearm);
        model.addMeleeWeapon(&m_jawsAndShield);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void SaurusGuard::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_celestitePolearm);
    visitor(m_celestitePolearmAlpha);
    visitor(m_jawsAndShield);
}

Unit *SaurusGuard::Create(const ParameterList &parameters)
{
    auto unit = new SaurusGuard();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool iconBearer = GetBoolParam("Stardrake Icon", parameters, false);
    bool wardrum = GetBoolParam("Wardrum", parameters, false);

    bool ok = unit->configure(numModels, iconBearer, wardrum);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SaurusGuard::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Saurus Guard", factoryMethod);
    }
}

int SaurusGuard::braveryModifier() const
{
    int modifier = Unit::braveryModifier();

    // Sworn Gardians
    auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 8.0f);
    for (auto ip : units)
    {
        if (ip->hasKeyword(HERO) && ip->hasKeyword(SERAPHON))
        {
            modifier += 2;
            break;
        }
    }
    return modifier;
}

int SaurusGuard::toSaveModifier(const Weapon *weapon) const
{
    int modifier = Unit::toSaveModifier(weapon);

    // Sworn Gardians
    auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 8.0f);
    for (auto ip : units)
    {
        if (ip->hasKeyword(HERO) && ip->hasKeyword(SERAPHON))
        {
            modifier += 1;
            break;
        }
    }

    // Stardrake Shields - ignore rend of -1 by cancelling it out.
    if (weapon->rend() == -1)
    {
        modifier = -weapon->rend();
    }

    return modifier;
}

} //namespace Seraphon
