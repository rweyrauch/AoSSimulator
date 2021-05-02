/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "slavestodarkness/DarkoathChieftain.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 90;

    bool DarkoathChieftain::s_registered = false;

    Unit *DarkoathChieftain::Create(const ParameterList &parameters) {
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        auto general = GetBoolParam("General", parameters, false);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);

        return new DarkoathChieftain(legion, trait, artefact, general);
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

    DarkoathChieftain::DarkoathChieftain(DamnedLegion legion, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SlavesToDarknessBase("Darkoath Chieftain", 6, g_wounds, 8, 5, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, EYE_OF_THE_GODS, HERO, DARKOATH_CHIEFTAIN};
        m_weapons = {&m_axe, &m_broadsword};
        m_battleFieldRole = Role::Leader;

        setDamnedLegion(legion);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_axe);
        model->addMeleeWeapon(&m_broadsword);
        addModel(model);
    }

    int DarkoathChieftain::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = SlavesToDarknessBase::extraAttacks(attackingModel, weapon, target);

        // Berserk Charge
        if (m_charged && (weapon->name() == m_broadsword.name())) extra += 3;

        return extra;
    }

    int DarkoathChieftain::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void DarkoathChieftain::onEnemyModelSlainWithWeapon(int numSlain, Unit *enemyUnit, const Weapon *weapon,
                                                        const Wounds &weaponDamage) {
        SlavesToDarknessBase::onEnemyModelSlainWithWeapon(numSlain, enemyUnit, weapon, weaponDamage);

        if (numSlain > 0) {
            m_deathblow = true;
        }
    }

    void DarkoathChieftain::onEndCombat(PlayerId player) {
        SlavesToDarknessBase::onEndCombat(player);

        if (m_deathblow) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
            for (auto unit : units) {
                unit->applyDamage({0, 1, Wounds::Source::Ability}, this);
            }
            m_deathblow = false;
        }
    }

} //namespace SlavesToDarkness
