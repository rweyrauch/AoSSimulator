/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/Epidemius.h"
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 60;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 200;

    bool EpidemiusTallymanOfNurgle::s_registered = false;

    Unit *EpidemiusTallymanOfNurgle::Create(const ParameterList &parameters) {
        auto unit = new EpidemiusTallymanOfNurgle();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        unit->setLegion(legion);

        unit->configure();
        return unit;
    }

    void EpidemiusTallymanOfNurgle::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    EpidemiusTallymanOfNurgle::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    EpidemiusTallymanOfNurgle::ComputePoints,
                    {
                            BoolParameter("General"),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Epidemius, Tallyman of Nurgle", factoryMethod);
        }
    }

    EpidemiusTallymanOfNurgle::EpidemiusTallymanOfNurgle() :
            NurgleBase("Epidemius, Tallyman of Nurgle", 4, g_wounds, 10, 4, false, g_pointsPerUnit),
            m_balesword(Weapon::Type::Melee, "Balesword", 1, 3, 3, 3, -1, RAND_D3),
            m_teeth(Weapon::Type::Melee, "Tiny Razor-sharp Teeth", 1, 5, 5, 5, 0, 1) {
        m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, HERO, EPIDEMIUS, TALLYMAN_OF_NURGLE};
        m_weapons = {&m_balesword, &m_teeth};
        m_battleFieldRole = Role::Leader;
    }

    void EpidemiusTallymanOfNurgle::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_balesword);
        model->addMeleeWeapon(&m_teeth);
        addModel(model);
    }

    Wounds EpidemiusTallymanOfNurgle::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Disgustingly Resilient
        return ignoreWounds(wounds, 5);
    }

    int EpidemiusTallymanOfNurgle::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Nurgle
