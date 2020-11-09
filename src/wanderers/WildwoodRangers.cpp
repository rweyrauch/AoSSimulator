/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <wanderers/WildwoodRangers.h>
#include <UnitFactory.h>

namespace Wanderers {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = 420;

    bool WildwoodRangers::s_registered = false;

    WildwoodRangers::WildwoodRangers() :
            Wanderer("Wildwood Rangers", 6, g_wounds, 7, 5, false),
            m_rangersDraich(Weapon::Type::Melee, "Ranger's Draich", 2, 2, 3, 3, -1, 1),
            m_wardensDraich(Weapon::Type::Melee, "Ranger's Draich", 2, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, WANDERER, WILDWOOD_RANGERS};
        m_weapons = {&m_rangersDraich, &m_wardensDraich};
    }

    bool WildwoodRangers::configure(int numModels, bool standardBearer, bool hornblower) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_standardBearer = standardBearer;
        m_hornblower = hornblower;

        auto warden = new Model(g_basesize, wounds());
        warden->addMeleeWeapon(&m_wardensDraich);
        addModel(warden);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_rangersDraich);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *WildwoodRangers::Create(const ParameterList &parameters) {
        auto unit = new WildwoodRangers();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool hornblower = GetBoolParam("Hornblower", parameters, false);

        bool ok = unit->configure(numModels, standardBearer, hornblower);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void WildwoodRangers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    WildwoodRangers::Create,
                    nullptr,
                    nullptr,
                    WildwoodRangers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblower"),
                    },
                    ORDER,
                    {WANDERER}
            };
            s_registered = UnitFactory::Register("Wildwood Rangers", factoryMethod);
        }
    }

    Wounds WildwoodRangers::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Guardians of the Kindreds
        if (target->hasKeyword(MONSTER)) {
            return {Dice::RollD3(), 0};
        }
        return Wanderer::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Rerolls WildwoodRangers::runRerolls() const {
        if (m_hornblower) {
            return Reroll_Failed;
        }
        return Wanderer::runRerolls();
    }

    int WildwoodRangers::braveryModifier() const {
        int modifier = Wanderer::braveryModifier();
        if (m_standardBearer) {
            modifier += 1;

            // if (Board::Instance()->unitInCover(this)) { modifier += 1; }
        }
        return modifier;
    }

    int WildwoodRangers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Wanderers
