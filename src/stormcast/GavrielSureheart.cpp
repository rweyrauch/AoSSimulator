/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/GavrielSureheart.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool GavrielSureheart::s_registered = false;

    GavrielSureheart::GavrielSureheart() :
            StormcastEternal("Gavriel Sureheart", 5, g_wounds, 9, 3, false),
            m_starboundBlade(Weapon::Type::Melee, "Tempest Axe", 1, 6, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HAMMERS_OF_SIGMAR, HERO, LORD_CELESTANT,
                      GAVRIEL_SUREHEART};
        m_weapons = {&m_starboundBlade};
        m_battleFieldRole = Leader;
    }

    bool GavrielSureheart::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_starboundBlade);
        addModel(model);

        m_commandAbilities.push_back(std::make_unique<BuffModifierCommandAbility>(this, "Once More, For Sigmar, Charge!", 12, 12, Phase::Charge,
                                                                        Charge_Distance, 3, Abilities::Target::SelfAndFriendly,
                                                                        std::vector<Keyword>(HAMMERS_OF_SIGMAR)));

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *GavrielSureheart::Create(const ParameterList &parameters) {
        auto unit = new GavrielSureheart();

        unit->setStormhost(Stormhost::Hammers_Of_Sigmar);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GavrielSureheart::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Gavriel Sureheart", factoryMethod);
        }
    }

    Rerolls GavrielSureheart::toSaveRerolls(const Weapon * /*weapon*/, const Unit* attacker) const {
        // Sigmarite Thundershield
        return Reroll_Ones;
    }

    Wounds GavrielSureheart::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto wounds = StormcastEternal::computeReturnedDamage(weapon, saveRoll);
        // Sigmarite Thundershield
        // 1 mortal wound for each save of a 6
        if (saveRoll == 6) {
            wounds += {0, 1};
        }
        return wounds;
    }

    int GavrielSureheart::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = StormcastEternal::extraAttacks(attackingModel, weapon, target);
        // Inescapable Vengeance
        if (m_charged) {
            attacks += 1;
        }
        return attacks;
    }

    int GavrielSureheart::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals