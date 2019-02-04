/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <idonethdeepkin/NamartiReavers.h>
#include <Board.h>
#include <Roster.h>
#include <UnitFactory.h>

namespace IdonethDeepkin
{

static FactoryMethod factoryMethod = {
    NamartiReavers::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = NamartiReavers::MIN_UNIT_SIZE}, NamartiReavers::MIN_UNIT_SIZE,
         NamartiReavers::MAX_UNIT_SIZE, NamartiReavers::MIN_UNIT_SIZE},
        {ParamType::Integer, "numIconBearers", {.m_intValue = 0}, 0, NamartiReavers::MAX_UNIT_SIZE/10}
    }
};

bool NamartiReavers::s_registered = false;

NamartiReavers::NamartiReavers() :
    Unit("Namarti Reavers", 8, WOUNDS, 6, 5, false),
    m_keeningBlade(Weapon::Type::Melee, "Keening Blade", 1, 2, 3, 4, 0, 1),
    m_whisperbowAimedFire(Weapon::Type::Missile, "Whisperbow: Aimed Fire", 18, 1, 4, 4, 0, 1),
    m_whisperbowStormFire(Weapon::Type::Missile, "Whisperbow: Storm Fire", 9, 3, 4, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, NAMARTI, REAVERS};
}

bool NamartiReavers::configure(int numModels, int numIconBearers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
        return false;

    if (numIconBearers > MAX_UNIT_SIZE / 10)
        return false;

    m_numIconBearers = numIconBearers;

    for (auto i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_whisperbowStormFire);
        model.addMissileWeapon(&m_whisperbowAimedFire);
        model.addMeleeWeapon(&m_keeningBlade);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    return true;
}

void NamartiReavers::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_whisperbowStormFire);
    visitor(&m_whisperbowAimedFire);
    visitor(&m_keeningBlade);
}

Unit *NamartiReavers::Create(const ParameterList &parameters)
{
    auto unit = new NamartiReavers();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    int numIconBearers = GetIntParam("numIconBearers", parameters, 0);

    bool ok = unit->configure(numModels, numIconBearers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void NamartiReavers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Namarti Reavers", factoryMethod);
    }
}

void NamartiReavers::shooting(PlayerId player)
{
    auto board = Board::Instance();
    auto otherRoster = board->getPlayerRoster(GetEnemyId(m_owningPlayer));

    // Fluid Firing Style
    auto nearestUnit = otherRoster ? otherRoster->nearestUnit(this) : nullptr;
    if (nearestUnit)
    {
        float rangeTo = distanceTo(nearestUnit);
        if (rangeTo < m_whisperbowStormFire.range())
        {
            m_whisperbowStormFire.activate(true);
            m_whisperbowAimedFire.activate(false);
        }
        else
        {
            m_whisperbowStormFire.activate(false);
            m_whisperbowAimedFire.activate(true);
        }
    }

    Unit::shooting(player);
}

} // namespace IdonethDeepkin
