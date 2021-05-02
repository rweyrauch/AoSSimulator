/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ExaltedHeroOfChaos.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 90;

    bool ExaltedHeroOfChaos::s_registered = false;

    Unit *ExaltedHeroOfChaos::Create(const ParameterList &parameters) {
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        auto general = GetBoolParam("General", parameters, false);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);

        return new ExaltedHeroOfChaos(legion, mark, trait, artefact, general);
    }

    void ExaltedHeroOfChaos::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ExaltedHeroOfChaos::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    ExaltedHeroOfChaos::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Exalted Hero of Chaos", factoryMethod);
        }
    }

    ExaltedHeroOfChaos::ExaltedHeroOfChaos(DamnedLegion legion, MarkOfChaos mark, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SlavesToDarknessBase("Exalted Hero of Chaos", 5, g_wounds, 8, 4, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, HERO, EXALTED_HERO_OF_CHAOS};
        m_weapons = {&m_blades};
        m_battleFieldRole = Role::Leader;

        setDamnedLegion(legion);
        setMarkOfChaos(mark);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blades);
        addModel(model);
    }

    Wounds ExaltedHeroOfChaos::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Dark Blessings
        Dice::RollResult mortalSaves;
        Dice::RollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.mortal -= mortalSaves.rollsGE(5);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }

    int ExaltedHeroOfChaos::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        // Glory-hungry Bladesman
        if (target->hasKeyword(HERO) || target->hasKeyword(MONSTER)) mod++;

        return mod;
    }

    int ExaltedHeroOfChaos::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void ExaltedHeroOfChaos::onEnemyModelSlainWithWeapon(int numSlain, Unit *enemyUnit, const Weapon *weapon,
                                                         const Wounds &weaponDamage) {
        SlavesToDarknessBase::onEnemyModelSlainWithWeapon(numSlain, enemyUnit, weapon, weaponDamage);

        if ((numSlain > 0) && (enemyUnit->hasKeyword(HERO) || enemyUnit->hasKeyword(MONSTER))) {
            m_thriceDamnedDagger = true;
        }
    }

    void ExaltedHeroOfChaos::onEndCombat(PlayerId player) {
        SlavesToDarknessBase::onEndCombat(player);

        if (m_thriceDamnedDagger) {
            heal(Dice::RollD3());
            m_thriceDamnedDagger = false;
        }
    }

} //namespace SlavesToDarkness
