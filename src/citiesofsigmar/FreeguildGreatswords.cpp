/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/FreeguildGreatswords.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = 360;

    bool FreeguildGreatswords::s_registered = false;

    Unit *FreeguildGreatswords::Create(const ParameterList &parameters) {
        auto unit = new FreeguildGreatswords();

        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool hornblower = GetBoolParam("Hornblower", parameters, true);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        bool ok = unit->configure(numModels, standard, hornblower);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string FreeguildGreatswords::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int FreeguildGreatswords::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void FreeguildGreatswords::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FreeguildGreatswords::Create,
                    FreeguildGreatswords::ValueToString,
                    FreeguildGreatswords::EnumStringToInt,
                    FreeguildGreatswords::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Freeguild Greatswords", factoryMethod);
        }
    }

    FreeguildGreatswords::FreeguildGreatswords() :
            CitizenOfSigmar("Freeguild Greatswords", 5, g_wounds, 6, 4, false),
            m_zweihander(Weapon::Type::Melee, "Zweihander", 1, 2, 3, 3, -1, 1),
            m_zweihanderChampion(Weapon::Type::Melee, "Zweihander", 1, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, FREEGUILD_GREATSWORDS};
        m_weapons = {&m_zweihander, &m_zweihanderChampion};
    }

    bool FreeguildGreatswords::configure(int numModels, bool standardBearer, bool hornblower) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the Champion
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMeleeWeapon(&m_zweihanderChampion);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_zweihander);
            if (standardBearer) {
                model->setName("Standard Bearer");
                standardBearer = false;
            }
            else if (hornblower) {
                model->setName("Hornblower");
                hornblower = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int FreeguildGreatswords::runModifier() const {
        auto mod = Unit::runModifier();
        if (isNamedModelAlive("Hornblower")) mod++;
        return mod;
    }

    int FreeguildGreatswords::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (isNamedModelAlive("Hornblower")) mod++;
        return mod;
    }

    int FreeguildGreatswords::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (isNamedModelAlive("Standard Bearer")) mod++;
        return mod;
    }

    Wounds
    FreeguildGreatswords::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Decapitating Swing
        auto damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
        if (hitRoll == 6) damage.mortal++;
        return damage;
    }

    int FreeguildGreatswords::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);
        // Oathsworn Honour Guard
        auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 18.0);
        if (unit && unit->hasKeyword(FREEGUILD)) mod++;
        return mod;
    }

    int FreeguildGreatswords::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar