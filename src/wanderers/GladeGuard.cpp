/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <wanderers/GladeGuard.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Wanderers
{

bool GladeGuard::s_registered = false;

GladeGuard::GladeGuard() :
    Wanderer("Glade Guard", 6, WOUNDS, 6, 6, false),
    m_longbow(Weapon::Type::Missile, "Longbow", 20, 1, 4, 4, 0, 1),
    m_longbowLord(Weapon::Type::Missile, "Longbow", 20, 2, 4, 4, 0, 1),
    m_gladeBlade(Weapon::Type::Melee, "Glade Blade", 1, 1, 5, 5, 0, 1)
{
    m_keywords = {ORDER, AELF, WANDERER, GLADE_GUARD};
    m_weapons = {&m_longbow, &m_longbowLord, &m_gladeBlade};
}

bool GladeGuard::configure(int numModels, bool pennantBearer, bool hornblower)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_pennantBearer = pennantBearer;
    m_hornblower = hornblower;

    auto lord = new Model(BASESIZE, wounds());
    lord->addMissileWeapon(&m_longbowLord);
    lord->addMeleeWeapon(&m_gladeBlade);
    addModel(lord);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_longbow);
        model->addMeleeWeapon(&m_gladeBlade);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *GladeGuard::Create(const ParameterList &parameters)
{
    auto unit = new GladeGuard();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool pennantBearer = GetBoolParam("Pennant Bearer", parameters, false);
    bool hornblower = GetBoolParam("Hornblower", parameters, false);

    bool ok = unit->configure(numModels, pennantBearer, hornblower);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void GladeGuard::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            GladeGuard::Create,
            nullptr,
            nullptr,
            GladeGuard::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", GladeGuard::MIN_UNIT_SIZE, GladeGuard::MIN_UNIT_SIZE,
                    GladeGuard::MAX_UNIT_SIZE, GladeGuard::MIN_UNIT_SIZE
                },
                {ParamType::Boolean, "Pennant Bearer", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                {ParamType::Boolean, "Hornblower", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
            },
            ORDER,
            { WANDERER }
        };
        s_registered = UnitFactory::Register("Glade Guard", factoryMethod);
    }
}

int GladeGuard::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = Wanderer::toHitModifier(weapon, target);
    // Peerless Archery
    if (weapon->isMissile() && remainingModels() >= 20)
    {
        auto closestUnit = Board::Instance()->getNearestUnit(this, owningPlayer());
        if ((closestUnit == nullptr) || (distanceTo(closestUnit) > 3))
        {
            modifier += 1;
        }
    }
    return modifier;
}

Rerolls GladeGuard::runRerolls() const
{
    if (m_hornblower)
    {
        return RerollFailed;
    }
    return Wanderer::runRerolls();
}

int GladeGuard::braveryModifier() const
{
    int modifier = Wanderer::braveryModifier();
    if (m_pennantBearer)
    {
        modifier += 1;

        // if (Board::Instance()->unitInCover(this)) { modifier += 1; }
    }
    return modifier;
}

void GladeGuard::onStartShooting(PlayerId player)
{
    Wanderer::onStartShooting(player);

    m_longbow.setRend(0);
    m_longbowLord.setRend(0);

    if (!m_usedArcaneBodkins)
    {
        if (m_shootingTarget)
        {
            // Arcane Bodkins
            m_usedArcaneBodkins = true;

            m_longbow.setRend(-3);
            m_longbowLord.setRend(-3);
        }
    }
}

int GladeGuard::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace Wanderers
