/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <eldritchcouncil/Swordmasters.h>
#include <UnitFactory.h>

namespace EldritchCouncil {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 160;
    static const int g_pointsMaxUnitSize = 420;

    bool Swordmasters::s_registered = false;

    Swordmasters::Swordmasters(int numModels, bool hornblower, bool standardBearer, int points) :
            Unit("Swordmasters", 6, g_wounds, 7, 4, false, points),
            m_greatsword(Weapon::Type::Melee, "Greatsword", 1, 2, 3, 3, -1, 1),
            m_greatswordLord(Weapon::Type::Melee, "Greatsword", 1, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, ELDRITCH_COUNCIL, SWORDMASTERS};
        m_weapons = {&m_greatsword, &m_greatswordLord};

        auto lord = new Model(g_basesize, wounds());
        lord->addMeleeWeapon(&m_greatswordLord);
        addModel(lord);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_greatsword);
            if (standardBearer) {
                model->setName(Model::StandardBearer);
                standardBearer = false;
            } else if (hornblower) {
                model->setName(Model::Hornblower);
                hornblower = false;
            }
            addModel(model);
        }
    }

    Unit *Swordmasters::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool hornblower = GetBoolParam("Hornblower", parameters, false);
        return new Swordmasters(numModels, hornblower, standardBearer, ComputePoints(parameters));
    }

    void Swordmasters::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    nullptr,
                    nullptr,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblower"),
                    },
                    ORDER,
                    {ELDRITCH_COUNCIL}
            };
            s_registered = UnitFactory::Register("Swordmasters", factoryMethod);
        }
    }

    Rerolls Swordmasters::toHitRerolls(const Weapon * /*weapon*/, const Unit * /*target*/) const {
        // A Blur of Blades
        return Rerolls::Ones;
    }

    Rerolls Swordmasters::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        // Deflect Shots
        if (weapon->isMissile()) {
            return Rerolls::Failed;
        }
        return Unit::toSaveRerolls(weapon, attacker);
    }

    int Swordmasters::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace EldritchCouncil