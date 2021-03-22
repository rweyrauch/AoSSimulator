/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/DarkoathChieftain.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 90;

    bool DarkoathChieftain::s_registered = false;

    Unit *DarkoathChieftain::Create(const ParameterList &parameters) {
        auto unit = new DarkoathChieftain();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void DarkoathChieftain::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    DarkoathChieftain::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    DarkoathChieftain::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Darkoath Chieftain", factoryMethod);
        }
    }

    DarkoathChieftain::DarkoathChieftain() :
            SlavesToDarknessBase("Darkoath Chieftain", 6, g_wounds, 8, 5, false),
            m_axe(Weapon::Type::Melee, "Warlord Axe", 1, 1, 3, 3, 0, 1),
            m_broadsword(Weapon::Type::Melee, "Cursed Broadsword", 1, 3, 4, 3, -1, 2) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, EYE_OF_THE_GODS, HERO, DARKOATH_CHIEFTAIN};
        m_weapons = {&m_axe, &m_broadsword};
        m_battleFieldRole = Role::Leader;
    }

    bool DarkoathChieftain::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_axe);
        model->addMeleeWeapon(&m_broadsword);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    int DarkoathChieftain::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = SlavesToDarknessBase::extraAttacks(attackingModel, weapon, target);

        // Berserk Charge
        if (m_charged && (weapon->name() == m_broadsword.name())) extra += 3;

        return extra;
    }

    int DarkoathChieftain::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace SlavesToDarkness
