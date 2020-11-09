/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/ValkiaTheBloody.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 140;

    bool ValkiaTheBloody::s_registered = false;

    ValkiaTheBloody::ValkiaTheBloody() :
            KhorneBase("Valkia the Bloody", 12, g_wounds, 9, 3, true),
            m_slaupnir(Weapon::Type::Melee, "Slaupnir", 2, 6, 3, 3, -2, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, VALKIA_THE_BLOODY};
        m_weapons = {&m_slaupnir};
        m_battleFieldRole = Leader;
    }

    bool ValkiaTheBloody::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_slaupnir);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *ValkiaTheBloody::Create(const ParameterList &parameters) {
        auto unit = new ValkiaTheBloody();

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

    void ValkiaTheBloody::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ValkiaTheBloody::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    ValkiaTheBloody::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Valkia the Bloody", factoryMethod);
        }
    }

    Wounds ValkiaTheBloody::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // The Spear Slaupnir
        if (m_charged && (weapon->name() == m_slaupnir.name())) {
            return {RAND_D3, 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int ValkiaTheBloody::targetWoundModifier(const Weapon *weapon, const Unit *attacker) const {
        // Daemonshield
        if (!weapon->isMissile()) {
            return -1;
        }
        return Unit::targetWoundModifier(weapon, attacker);
    }

    int ValkiaTheBloody::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Khorne