/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Cockatrice.h>
#include <UnitFactory.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 60;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 90;

    bool Cockatrice::s_registered = false;

    Cockatrice::Cockatrice() :
            BeastsOfChaosBase("Cockatrice", 12, g_wounds, 5, 6, true),
            m_petrifyingGaze(Weapon::Type::Missile, "Petrifying Gaze", 10, 0, 0, 0, 0, 0),
            m_viciousBeak(Weapon::Type::Melee, "Vicious Beak", 2, 2, 4, 3, -1, RAND_D3),
            m_swordlikeTalons(Weapon::Type::Melee, "Sword-like Talons", 1, 4, 4, 4, 0, 1) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, MONSTERS_OF_CHAOS, MONSTER, COCKATRICE};
        m_weapons = {&m_petrifyingGaze, &m_viciousBeak, &m_swordlikeTalons};
    }

    bool Cockatrice::configure() {
        auto model = new Model(g_basesize, wounds());

        // NOTE: Petrifying Gaze attack is special, do not treat it as a weapon

        model->addMeleeWeapon(&m_viciousBeak);
        model->addMeleeWeapon(&m_swordlikeTalons);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *Cockatrice::Create(const ParameterList &parameters) {
        auto unit = new Cockatrice();

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        unit->setGreatfray(fray);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Cockatrice::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Cockatrice", factoryMethod);
        }
    }

    int Cockatrice::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Unit::extraAttacks(attackingModel, weapon, target);

        // Maddened Ferocity
        if (m_charged && (weapon->name() == m_swordlikeTalons.name())) {
            extra += 4;
        }
        return extra;
    }

    void Cockatrice::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        if (player == owningPlayer()) {
            // Petrifying Gaze
            if (m_shootingTarget) {
                if (distanceTo(m_shootingTarget) <= (double) m_petrifyingGaze.range()) {
                    if (Dice::RollD6() >= 4) {
                        Wounds gazeDamage = {0, Dice::RollD6()};
                        m_shootingTarget->applyDamage(gazeDamage);
                    }
                }
            }
        }
    }

    int Cockatrice::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace BeastsOfChaos
