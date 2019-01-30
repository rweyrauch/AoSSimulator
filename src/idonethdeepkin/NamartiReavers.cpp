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

Weapon NamartiReavers::s_keeningBlade(Weapon::Type::Melee, "Keening Blade", 1, 2, 3, 4, 0, 1);
Weapon NamartiReavers::s_whisperbowAimedFire(Weapon::Type::Missile, "Whisperbow: Aimed Fire", 18, 1, 4, 4, 0, 1);
Weapon NamartiReavers::s_whisperbowStormFire(Weapon::Type::Missile, "Whisperbow: Storm Fire", 9, 3, 4, 4, 0, 1);

bool NamartiReavers::s_registered = false;

NamartiReavers::NamartiReavers() :
    Unit("Namarti Reavers", 8, WOUNDS, 6, 5, false)
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

    m_pWhisperbowAimedFire = new Weapon(s_whisperbowAimedFire);
    m_pWhisperbowStormFire = new Weapon(s_whisperbowStormFire);

    for (auto i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(m_pWhisperbowStormFire);
        model.addMissileWeapon(m_pWhisperbowAimedFire);
        model.addMeleeWeapon(&s_keeningBlade);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    return true;
}

void NamartiReavers::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&s_whisperbowStormFire);
    visitor(&s_whisperbowAimedFire);
    visitor(&s_keeningBlade);
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
    PlayerId otherPlayer = PlayerId::Red;
    if (player == PlayerId::Red)
        otherPlayer = PlayerId::Blue;
    auto otherRoster = board->getPlayerRoster(otherPlayer);

    // Fluid Firing Style
    auto nearestUnit = otherRoster ? otherRoster->nearestUnit(this) : nullptr;
    if (nearestUnit)
    {
        float rangeTo = distanceTo(nearestUnit);
        if (rangeTo < m_pWhisperbowStormFire->range())
        {
            m_pWhisperbowStormFire->activate(true);
            m_pWhisperbowAimedFire->activate(false);
        }
        else
        {
            m_pWhisperbowStormFire->activate(false);
            m_pWhisperbowAimedFire->activate(true);
        }
    }

    Unit::shooting(player);
}

} // namespace IdonethDeepkin
