/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "nighthaunt/LordExecutioner.h"
#include "NighthauntPrivate.h"

namespace Nighthaunt {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 80;

    bool LordExecutioner::s_registered = false;

    Unit *LordExecutioner::Create(const ParameterList &parameters) {
        auto procession = (Procession) GetEnumParam("Procession", parameters, g_processions[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new LordExecutioner(procession, trait, artefact, general);
    }

    void LordExecutioner::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LordExecutioner::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    LordExecutioner::ComputePoints,
                    {
                            EnumParameter("Procession", g_processions[0], g_processions),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")

                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Lord Executioner", factoryMethod);
        }
    }

    LordExecutioner::LordExecutioner(Procession procession, CommandTrait trait, Artefact artefact, bool isGeneral) :
            Nighthaunt(procession, "Lord Executioner", 6, g_wounds, 10, 4, true, g_pointsPerUnit),
            m_greataxe(Weapon::Type::Melee, "Decapitating Greataxe", 1, 3, 3, 3, -2, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, LORD_EXECUTIONER};
        m_weapons = {&m_greataxe};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_greataxe);
        addModel(model);
    }

    Wounds LordExecutioner::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Beheading Strike
        if ((woundRoll == 6) && (weapon->name() == m_greataxe.name())) {
            return {weapon->damage() + 2, 0};
        }
        return Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void LordExecutioner::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        // Staring Death in the Face
        auto unit = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), HERO, 3.0);
        if (unit) {
            unit->buffModifier(Attribute::To_Hit_Melee, -1, {GamePhase::Battleshock, m_battleRound, player});
        }

    }

    Wounds LordExecutioner::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto unsavedWounds = Nighthaunt::applyWoundSave(wounds, attackingUnit);

        // Disembodied Skulls
        if (unsavedWounds.mortal) {
            Dice::RollResult result;
            Dice::RollD6(unsavedWounds.mortal, result);
            unsavedWounds.mortal -= result.rollsGE(5);
            unsavedWounds.mortal = std::max(0, unsavedWounds.mortal);
        }
        return unsavedWounds;
    }

    int LordExecutioner::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Nighthaunt
