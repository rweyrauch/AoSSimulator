/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <wanderers/EternalGuard.h>
#include <UnitFactory.h>

namespace Wanderers {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = 210;

    bool EternalGuard::s_registered = false;

    EternalGuard::EternalGuard(int numModels, bool standardBearer, bool hornblower, bool gladeShields, int points) :
            Wanderer("Eternal Guard", 6, g_wounds, 7, 5, false, points),
            m_spearStave(Weapon::Type::Melee, "Spear-stave", 2, 1, 4, 4, 0, 1),
            m_spearStaveWarden(Weapon::Type::Melee, "Spear-stave", 2, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, WANDERER, ETERNAL_GUARD};
        m_weapons = {&m_spearStave, &m_spearStaveWarden};

        m_gladeShields = gladeShields;

        auto lord = new Model(g_basesize, wounds());
        lord->addMeleeWeapon(&m_spearStaveWarden);
        addModel(lord);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_spearStave);
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

    Unit *EternalGuard::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool hornblower = GetBoolParam("Hornblower", parameters, false);
        bool shields = GetBoolParam("Glade Shields", parameters, false);
        return new EternalGuard(numModels, standardBearer, hornblower, shields, ComputePoints(parameters));
    }

    void EternalGuard::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    EternalGuard::Create,
                    nullptr,
                    nullptr,
                    EternalGuard::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblower"),
                            BoolParameter("Glade Shields"),
                    },
                    ORDER,
                    {WANDERER}
            };
            s_registered = UnitFactory::Register("Eternal Guard", factoryMethod);
        }
    }

    Rerolls EternalGuard::runRerolls() const {
        if (isNamedModelAlive(Model::Hornblower)) {
            return Rerolls::Failed;
        }
        return Wanderer::runRerolls();
    }

    int EternalGuard::braveryModifier() const {
        int modifier = Wanderer::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) {
            modifier += 1;

            // if (Board::Instance()->unitInCover(this)) { modifier += 1; }
        }
        return modifier;
    }

    Rerolls EternalGuard::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        if (m_gladeShields) {
            // if (Board::Instance()->unitInCover(this)) { return RerollOnesAndTwos; }
            return Rerolls::Ones;
        }
        return Wanderer::toSaveRerolls(weapon, attacker);
    }

    void EternalGuard::onStartHero(PlayerId /*player*/) {
        m_fortressModifier = 0;
        m_canMove = true;

        if (m_meleeTarget) {
            m_fortressModifier = 1;
            m_canMove = false;
        }
    }

    int EternalGuard::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Wanderers
