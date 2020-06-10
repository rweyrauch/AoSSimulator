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
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 210;

    bool EternalGuard::s_registered = false;

    EternalGuard::EternalGuard() :
            Wanderer("Eternal Guard", 6, WOUNDS, 7, 5, false),
            m_spearStave(Weapon::Type::Melee, "Spear-stave", 2, 1, 4, 4, 0, 1),
            m_spearStaveWarden(Weapon::Type::Melee, "Spear-stave", 2, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, WANDERER, ETERNAL_GUARD};
        m_weapons = {&m_spearStave, &m_spearStaveWarden};
    }

    bool EternalGuard::configure(int numModels, bool standardBearer, bool hornblower, bool gladeShields) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_standardBearer = standardBearer;
        m_hornblower = hornblower;
        m_gladeShields = gladeShields;

        auto lord = new Model(BASESIZE, wounds());
        lord->addMeleeWeapon(&m_spearStaveWarden);
        addModel(lord);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_spearStave);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *EternalGuard::Create(const ParameterList &parameters) {
        auto unit = new EternalGuard();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool hornblower = GetBoolParam("Hornblower", parameters, false);
        bool shields = GetBoolParam("Glade Shields", parameters, false);

        bool ok = unit->configure(numModels, standardBearer, hornblower, shields);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void EternalGuard::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    EternalGuard::Create,
                    nullptr,
                    nullptr,
                    EternalGuard::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
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
        if (m_hornblower) {
            return RerollFailed;
        }
        return Wanderer::runRerolls();
    }

    int EternalGuard::braveryModifier() const {
        int modifier = Wanderer::braveryModifier();
        if (m_standardBearer) {
            modifier += 1;

            // if (Board::Instance()->unitInCover(this)) { modifier += 1; }
        }
        return modifier;
    }

    Rerolls EternalGuard::toSaveRerolls(const Weapon *weapon) const {
        if (m_gladeShields) {
            // if (Board::Instance()->unitInCover(this)) { return RerollOnesAndTwos; }
            return RerollOnes;
        }
        return Wanderer::toSaveRerolls(weapon);
    }

    void EternalGuard::onStartHero(PlayerId /*player*/) {
        m_fortressModifier = 0;
        m_canMove = true;

        if (m_meleeTarget) {
            m_fortressModifier = 1;
            m_canMove = false;
        }
    }

    int EternalGuard::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace Wanderers
