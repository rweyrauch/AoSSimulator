/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/ThornsOfTheBriarQueen.h>
#include <UnitFactory.h>
#include <iostream>

namespace Nighthaunt {
    static const int g_numModels = 6;
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_pointsPerUnit = 75;

    bool ThornsOfTheBriarQueen::s_registered = false;

    ThornsOfTheBriarQueen::ThornsOfTheBriarQueen() :
            Nighthaunt("Thorns of the Briar Queen", 6, g_wounds, 6, 5, true), // todo: bravery 6 when no Dreadwarden
            m_malignantWeapon(Weapon::Type::Melee, "Malignant Weapon", 1, 2, 4, 4, 0, 1),
            m_malignantWeaponVarclav(Weapon::Type::Melee, "Malignant Weapon", 1, 3, 4, 4, 0, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, CHAINRASP_HORDE, THORNS_OF_THE_BRIAR_QUEEN};
        m_weapons = {&m_malignantWeapon, &m_malignantWeaponVarclav};
        m_battleFieldRole = Battleline;
    }

    bool ThornsOfTheBriarQueen::configure() {

        auto warden = new Model(g_basesize, wounds());
        warden->addMeleeWeapon(&m_malignantWeaponVarclav);
        addModel(warden);

        for (auto i = 1; i < g_numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_malignantWeapon);
            addModel(model);
        }

        m_points = ComputePoints(g_numModels);

        return true;
    }

    Unit *ThornsOfTheBriarQueen::Create(const ParameterList &parameters) {
        auto unit = new ThornsOfTheBriarQueen();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ThornsOfTheBriarQueen::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ThornsOfTheBriarQueen::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    ThornsOfTheBriarQueen::ComputePoints,
                    {
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };

            s_registered = UnitFactory::Register("Thorns of the Briar Queen", factoryMethod);
        }
    }

    Rerolls ThornsOfTheBriarQueen::toWoundRerolls(const Weapon *weapon, const Unit *unit) const {
        // Chilling Horde
        if (remainingModels() >= 10) {
            return Reroll_Ones;
        }

        return Nighthaunt::toHitRerolls(weapon, unit);
    }

    int ThornsOfTheBriarQueen::ComputePoints(int numModels) {
        return g_pointsPerUnit;
    }

} // namespace Nighthaunt
