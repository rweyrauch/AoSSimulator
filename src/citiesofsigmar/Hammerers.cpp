/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/Hammerers.h>
#include <UnitFactory.h>
#include <Board.h>

namespace CitiesOfSigmar {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    bool Hammerers::s_registered = false;

    Hammerers::Hammerers() :
            CitizenOfSigmar("Hammerers", 4, WOUNDS, 7, 4, false),
            m_greatHammer(Weapon::Type::Melee, "Gromril Great Hammer", 1, 2, 3, 3, -1, 1),
            m_greatHammerKeeper(Weapon::Type::Melee, "Gromril Great Hammer", 1, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, HAMMERERS};
        m_weapons = {&m_greatHammer, &m_greatHammerKeeper};
    }

    bool Hammerers::configure(int numModels, bool standardBearer, bool musician) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_standardBearer = standardBearer;
        m_musician = musician;

        auto keeper = new Model(BASESIZE, wounds());
        keeper->addMeleeWeapon(&m_greatHammerKeeper);
        addModel(keeper);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_greatHammer);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Hammerers::Create(const ParameterList &parameters) {
        auto unit = new Hammerers();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool musician = GetBoolParam("Musician", parameters, false);

        auto city = (City) GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
        unit->setCity(city);

        bool ok = unit->configure(numModels, standardBearer, musician);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Hammerers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Hammerers::Create,
                    CitizenOfSigmar::ValueToString,
                    CitizenOfSigmar::EnumStringToInt,
                    Hammerers::ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Boolean, "Standard Bearer", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                            {ParamType::Boolean, "Musician", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                            {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal,
                             CitizenOfSigmar::TempestsEye, 1},
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Hammerers", factoryMethod);
        }
    }

    bool Hammerers::battleshockRequired() const {
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 16.0f);
        for (auto ip : units) {
            // Kingsguard
            if (ip->hasKeyword(DISPOSSESSED) && ip->hasKeyword(HERO))
                return false;
        }
        return true;
    }

    int Hammerers::runModifier() const {
        auto mod = Unit::runModifier();
        if (m_musician) mod++;
        return mod;
    }

    int Hammerers::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (m_musician) mod++;
        return mod;
    }

    int Hammerers::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (m_standardBearer) mod++;
        return mod;
    }

    int Hammerers::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace CitiesOfSigmar
