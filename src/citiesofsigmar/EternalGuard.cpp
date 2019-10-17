/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/EternalGuard.h>
#include <UnitFactory.h>

namespace CitiesOfSigmar
{

static FactoryMethod factoryMethod = {
    EternalGuard::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", EternalGuard::MIN_UNIT_SIZE, EternalGuard::MIN_UNIT_SIZE,
            EternalGuard::MAX_UNIT_SIZE, EternalGuard::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Standard Bearer", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
        {ParamType::Boolean, "Hornblower", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
        {ParamType::Boolean, "Glade Shields", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool EternalGuard::s_registered = false;

EternalGuard::EternalGuard() :
    CitizenOfSigmar("Eternal Guard", 6, WOUNDS, 7, 4, false),
    m_spearStave(Weapon::Type::Melee, "Spear-stave", 2, 2, 3, 4, 0, 1),
    m_spearStaveWarden(Weapon::Type::Melee, "Spear-stave", 2, 3, 3, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, WANDERER, ETERNAL_GUARD};
}

bool EternalGuard::configure(int numModels, bool standardBearer, bool hornblower, bool gladeShields)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_standardBearer = standardBearer;
    m_hornblower = hornblower;
    m_gladeShields = gladeShields;

    Model lord(BASESIZE, WOUNDS);
    lord.addMeleeWeapon(&m_spearStaveWarden);
    addModel(lord);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_spearStave);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void EternalGuard::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_spearStave);
    visitor(&m_spearStaveWarden);
}

Unit *EternalGuard::Create(const ParameterList &parameters)
{
    auto unit = new EternalGuard();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
    bool hornblower = GetBoolParam("Hornblower", parameters, false);
    bool shields = GetBoolParam("Glade Shields", parameters, false);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels, standardBearer, hornblower, shields);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void EternalGuard::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Eternal Guard", factoryMethod);
    }
}

Rerolls EternalGuard::runRerolls() const
{
    if (m_hornblower)
    {
        return RerollFailed;
    }
    return CitizenOfSigmar::runRerolls();
}

int EternalGuard::braveryModifier() const
{
    int modifier = CitizenOfSigmar::braveryModifier();
    if (m_standardBearer)
    {
        modifier += 1;

        // if (Board::Instance()->unitInCover(this)) { modifier += 1; }
    }
    return modifier;
}

Rerolls EternalGuard::toSaveRerolls(const Weapon *weapon) const
{
    if (m_gladeShields)
    {
        // if (Board::Instance()->unitInCover(this)) { return RerollOnesAndTwos; }
        return RerollOnes;
    }
    return CitizenOfSigmar::toSaveRerolls(weapon);
}

void EternalGuard::onStartHero(PlayerId player)
{
    m_fortressModifier = 0;
    m_canMove = true;

    if (m_meleeTarget)
    {
        m_fortressModifier = 1;
        m_canMove = false;
    }
}

} // namespace CitiesOfSigmar
