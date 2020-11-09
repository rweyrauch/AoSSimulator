/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Skulltaker.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool Skulltaker::s_registered = false;

    Skulltaker::Skulltaker() :
            KhorneBase("Skulltaker", 5, g_wounds, 10, 4, false),
            m_slayerSword(Weapon::Type::Melee, "The Slayer Sword", 1, 3, 3, 3, -1, 3) {
        m_keywords = {CHAOS, DAEMON, BLOODLETTER, KHORNE, HERO, HERALD_OF_KHORNE, SKULLTAKER};
        m_weapons = {&m_slayerSword};
        m_battleFieldRole = Leader;
    }

    bool Skulltaker::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_slayerSword);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *Skulltaker::Create(const ParameterList &parameters) {
        auto unit = new Skulltaker();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Skulltaker::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Skulltaker::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    Skulltaker::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Skulltaker", factoryMethod);
        }
    }

    Wounds Skulltaker::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Decapitating Strike
        if (hitRoll == 6) {
            return {weapon->damage(), 3};
        }
        return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Rerolls Skulltaker::toSaveRerolls(const Weapon * /*weapon*/) const {
        // Cloak of Skulls
        return Reroll_Failed;
    }

    Rerolls Skulltaker::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Heroes' Bane
        if (target->hasKeyword(HERO)) {
            return Reroll_Failed;
        }
        return KhorneBase::toHitRerolls(weapon, target);
    }

    Rerolls Skulltaker::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Heroes' Bane
        if (target->hasKeyword(HERO)) {
            return Reroll_Failed;
        }
        return KhorneBase::toWoundRerolls(weapon, target);
    }

    int Skulltaker::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }


} // namespace Khorne