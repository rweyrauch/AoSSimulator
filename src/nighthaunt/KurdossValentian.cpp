/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "nighthaunt/KurdossValentian.h"

namespace Nighthaunt {
    static const int g_basesize = 60;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 180;

    bool KurdossValentian::s_registered = false;

    Unit *KurdossValentian::Create(const ParameterList &parameters) {
        auto unit = new KurdossValentian();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void KurdossValentian::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    KurdossValentian::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    KurdossValentian::ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Kurdoss Valentian", factoryMethod);
        }
    }

    KurdossValentian::KurdossValentian() :
            Nighthaunt("Kurdoss Valentian", 6, g_wounds, 10, 4, true),
            m_sceptre(Weapon::Type::Melee, "Sepulchral Sceptre", 1, 5, 3, 3, -2, RAND_D3),
            m_claws(Weapon::Type::Melee, "Wraith Herald's Spectral Claws", 1, 6, 4, 4, 0, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, KURDOSS_VALENTIAN};
        m_weapons = {&m_sceptre, &m_claws};
        m_battleFieldRole = Leader;
    }

    bool KurdossValentian::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_sceptre);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    Wounds KurdossValentian::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll == 6) && (weapon->name() == m_claws.name())) {
            return {0, 1};
        }

        // Soul-crushing Smite
        if ((woundRoll == 6) && (weapon->name() == m_sceptre.name())) {
            return {Dice::RollD6(), 0};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Rerolls KurdossValentian::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (target->isGeneral() && (weapon->name() == m_sceptre.name())) {
            return Reroll_Failed;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    int KurdossValentian::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Nighthaunt
