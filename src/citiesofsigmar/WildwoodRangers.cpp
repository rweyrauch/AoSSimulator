/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/WildwoodRangers.h>
#include <UnitFactory.h>

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    WildwoodRangers::Create,
    CitizenOfSigmar::ValueToString,
    CitizenOfSigmar::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", WildwoodRangers::MIN_UNIT_SIZE, WildwoodRangers::MIN_UNIT_SIZE,
            WildwoodRangers::MAX_UNIT_SIZE, WildwoodRangers::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Standard Bearer", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
        {ParamType::Boolean, "Hornblower", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool WildwoodRangers::s_registered = false;

WildwoodRangers::WildwoodRangers() :
    CitizenOfSigmar("Wildwood Rangers", 6, WOUNDS, 7, 5, false),
    m_rangersDraich(Weapon::Type::Melee, "Ranger's Draich", 2, 2, 3, 3, -1, 1),
    m_wardensDraich(Weapon::Type::Melee, "Ranger's Draich", 2, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, WANDERER, WILDWOOD_RANGERS};
}

bool WildwoodRangers::configure(int numModels, bool standardBearer, bool hornblower)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_standardBearer = standardBearer;
    m_hornblower = hornblower;

    auto warden = new Model(BASESIZE, WOUNDS);
    warden->addMeleeWeapon(&m_wardensDraich);
    addModel(warden);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_rangersDraich);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void WildwoodRangers::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_rangersDraich);
    visitor(m_wardensDraich);
}

Unit *WildwoodRangers::Create(const ParameterList &parameters)
{
    auto unit = new WildwoodRangers();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
    bool hornblower = GetBoolParam("Hornblower", parameters, false);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels, standardBearer, hornblower);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void WildwoodRangers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Wildwood Rangers", factoryMethod);
    }
}

Wounds WildwoodRangers::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Guardians of the Kindred
    if (target->hasKeyword(MONSTER))
    {
        return {2, 0};
    }
    return CitizenOfSigmar::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int WildwoodRangers::runModifier() const
{
    auto mod = Unit::runModifier();
    if (m_hornblower) mod++;
    return mod;
}

int WildwoodRangers::chargeModifier() const
{
    auto mod = Unit::chargeModifier();
    if (m_hornblower) mod++;
    return mod;
}

int WildwoodRangers::braveryModifier() const
{
    auto mod = Unit::braveryModifier();
    if (m_standardBearer) mod++;
    return mod;
}

} // namespace Wanderers
