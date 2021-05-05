/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosLordOnDaemonicMount.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 170;

    bool ChaosLordOnDaemonicMount::s_registered = false;

    Unit *ChaosLordOnDaemonicMount::Create(const ParameterList &parameters) {
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        auto general = GetBoolParam("General", parameters, false);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);

        return new ChaosLordOnDaemonicMount(legion, mark, trait, artefact, general);
    }

    void ChaosLordOnDaemonicMount::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ChaosLordOnDaemonicMount::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    ChaosLordOnDaemonicMount::ComputePoints,
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
            s_registered = UnitFactory::Register("Chaos Lord On Daemonic Mount", factoryMethod);
        }
    }

    ChaosLordOnDaemonicMount::ChaosLordOnDaemonicMount(DamnedLegion legion, MarkOfChaos mark, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SlavesToDarknessBase(legion, "Chaos Lord On Daemonic Mount", 10, g_wounds, 8, 4, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, DAEMON, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, HERO, CHAOS_LORD};
        m_weapons = {&m_hammer, &m_hooves};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_hooves.setMount(true);

        setMarkOfChaos(mark);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_hammer);
        model->addMeleeWeapon(&m_hooves);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Wounds ChaosLordOnDaemonicMount::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto savedWounds = SlavesToDarknessBase::applyWoundSave(wounds, attackingUnit);
        Dice::RollResult result;
        // Chaos Runeshield
        Dice::RollD6(savedWounds.mortal, result);
        savedWounds.mortal -= result.rollsGE(5);
        return savedWounds;
    }

    Wounds
    ChaosLordOnDaemonicMount::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = SlavesToDarknessBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
        // Cursed Warhammer
        if ((hitRoll == 6) && (weapon->name() == m_hooves.name())) {
            damage.mortal = 2;
            damage.normal = 0;
        }
        return damage;
    }

    int ChaosLordOnDaemonicMount::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void ChaosLordOnDaemonicMount::onEnemyModelSlainWithWeapon(int numSlain, Unit *enemyUnit, const Weapon *weapon,
                                                               const Wounds &weaponDamage) {
        SlavesToDarknessBase::onEnemyModelSlainWithWeapon(numSlain, enemyUnit, weapon, weaponDamage);

        if ((weapon->name() == m_hammer.name()) && (numSlain > 0)) {
            m_fueledByCarnage = true;
        }
    }

    void ChaosLordOnDaemonicMount::onEndCombat(PlayerId player) {
        SlavesToDarknessBase::onEndCombat(player);

        if (m_fueledByCarnage) {
            heal(Dice::RollD3());
            m_fueledByCarnage = false;
        }
    }

} //namespace SlavesToDarkness
