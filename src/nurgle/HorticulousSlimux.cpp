/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/HorticulousSlimux.h"
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 105; // x70 oval
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 220;

    bool HorticulousSlimux::s_registered = false;

    Unit *HorticulousSlimux::Create(const ParameterList &parameters) {
        auto unit = new HorticulousSlimux();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        unit->setLegion(legion);

        unit->configure();
        return unit;
    }

    void HorticulousSlimux::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    HorticulousSlimux::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    HorticulousSlimux::ComputePoints,
                    {
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Horticulous Slimux", factoryMethod);
        }
    }

    HorticulousSlimux::HorticulousSlimux() :
            NurgleBase("Horticulous Slimux", 5, g_wounds, 10, 3, false, g_pointsPerUnit),
            m_shears(Weapon::Type::Melee, "Lopping Shears", 1, 3, 3, 3, -1, RAND_D3),
            m_jaws(Weapon::Type::Melee, "Mulch's Slime-encrusted Jaws", 1, RAND_D3, 3, 3, -2, 2) {
        m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, HERO, HORTICULOUS_SLIMUX};
        m_weapons = {&m_shears, &m_jaws};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_jaws.setMount(true);
        s_globalChargeReroll.connect(this, &HorticulousSlimux::beastHandlerChargeReroll, &m_beastHandlerChargeSlot);
        s_globalToHitReroll.connect(this, &HorticulousSlimux::beastHandlerToHitRerolls, &m_beastHandlerToHitSlot);
    }

    HorticulousSlimux::~HorticulousSlimux() {
        m_beastHandlerChargeSlot.disconnect();
        m_beastHandlerToHitSlot.disconnect();
    }

    void HorticulousSlimux::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_shears);
        model->addMeleeWeapon(&m_jaws);
        addModel(model);
    }

    Wounds HorticulousSlimux::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Disgustingly Resilient
        return ignoreWounds(wounds, 5);
    }

    Rerolls HorticulousSlimux::beastHandlerChargeReroll(const Unit *unit) {
        if (isFriendly(unit) && (distanceTo(unit) <= 7.0)) {
            if (unit->hasKeyword(BEASTS_OF_NURGLE)) return Rerolls::Ones;
        }
        return Rerolls::None;
    }

    Rerolls
    HorticulousSlimux::beastHandlerToHitRerolls(const Unit *attacker, const Weapon * /*weapon*/,
                                                const Unit * /*target*/) {
        if (isFriendly(attacker) && (distanceTo(attacker) <= 7.0)) {
            if (attacker->hasKeyword(BEASTS_OF_NURGLE)) return Rerolls::Ones;
        }
        return Rerolls::None;
    }

    int HorticulousSlimux::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Nurgle
