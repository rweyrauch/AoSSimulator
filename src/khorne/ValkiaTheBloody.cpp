/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/ValkiaTheBloody.h>
#include <UnitFactory.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 140;

    bool ValkiaTheBloody::s_registered = false;

    ValkiaTheBloody::ValkiaTheBloody(SlaughterHost host, bool isGeneral) :
            KhorneBase("Valkia the Bloody", 12, g_wounds, 9, 3, true, g_pointsPerUnit) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, VALKIA_THE_BLOODY};
        m_weapons = {&m_slaupnir};
        m_battleFieldRole = Role::Leader;

        s_globalBattleshockReroll.connect(this, &ValkiaTheBloody::gazeOfKhorne, &m_gazeOfKhorne);
        s_globalBattleshockFleeModifier.connect(this, &ValkiaTheBloody::gazeOfKhorneFlee, &m_gazeOfKhorneFlee);

        setSlaughterHost(host);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_slaupnir);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    ValkiaTheBloody::~ValkiaTheBloody() {
        m_gazeOfKhorne.disconnect();
        m_gazeOfKhorneFlee.disconnect();
    }

    Unit *ValkiaTheBloody::Create(const ParameterList &parameters) {
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new ValkiaTheBloody(host, general);
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

    Wounds ValkiaTheBloody::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // The Spear Slaupnir
        if (m_charged && (weapon->name() == m_slaupnir.name())) {
            return {RAND_D3, 1};
        }
        return KhorneBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int ValkiaTheBloody::targetWoundModifier(const Weapon *weapon, const Unit *attacker) const {
        // Daemonshield
        if (!weapon->isMissile()) {
            return -1;
        }
        return KhorneBase::targetWoundModifier(weapon, attacker);
    }

    int ValkiaTheBloody::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    Rerolls ValkiaTheBloody::gazeOfKhorne(const Unit *unit) {
        if (isFriendly(unit) && unit->hasKeyword(KHORNE) && unit->hasKeyword(MORTAL) && (distanceTo(unit) < 16.0)) {
            m_usedGazeOfKhorneReroll = unit;
            return Rerolls::Failed;
        }
        return Rerolls::None;
    }

    int ValkiaTheBloody::gazeOfKhorneFlee(const Unit *unit, int roll) {
        if (unit && (unit == m_usedGazeOfKhorneReroll)) {
            m_usedGazeOfKhorneReroll = nullptr;
            return Dice::RollD3();
        }
        return 0;
    }

} // namespace Khorne