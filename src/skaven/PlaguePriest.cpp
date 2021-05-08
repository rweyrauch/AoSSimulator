/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/PlaguePriest.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SkavenPrivate.h"

namespace Skaven {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 80;

    bool PlaguePriest::s_registered = false;

    Unit *PlaguePriest::Create(const ParameterList &parameters) {
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_pestilensCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_pestilensArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new PlaguePriest(trait, artefact, general);
    }

    void PlaguePriest::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Command Trait", g_pestilensCommandTraits[0], g_pestilensCommandTraits),
                            EnumParameter("Artefact", g_pestilensArtefacts[0], g_pestilensArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Plague Priest", factoryMethod);
        }
    }

    PlaguePriest::PlaguePriest(CommandTrait trait, Artefact artefact, bool isGeneral) :
            Skaventide("Plague Priest", 6, g_wounds, 6, 5, false, g_pointsPerUnit),
            m_staff(Weapon::Type::Melee, "Warpstone-tipped Staff", 2, 1, 4, 3, -1, RAND_D3),
            m_censer(Weapon::Type::Melee, "Plague Censer", 2, 2, 4, 3, -1, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, NURGLE, CLANS_PESTILENS, HERO, PRIEST, PLAGUE_PRIEST};
        m_weapons = {&m_staff, &m_censer};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_censer);
        addModel(model);
    }

    void PlaguePriest::onEndCombat(PlayerId player) {
        Skaventide::onEndCombat(player);

        // Poisonous Fumes
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        for (auto unit : units) {
            if (!unit->hasKeyword(CLANS_PESTILENS)) {
                int mortalWounds = 0;
                int roll = Dice::RollD6();
                if (roll == 6) mortalWounds = Dice::RollD3();
                else if (roll >= 4) mortalWounds = 1;

                unit->applyDamage({0, mortalWounds}, this);
            }
        }
    }

    int PlaguePriest::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Skaventide::extraAttacks(attackingModel, weapon, target);

        // Frenzied Assault
        if (m_charged) extra++;

        return extra;
    }

    void PlaguePriest::onStartHero(PlayerId player) {
        Skaventide::onStartHero(player);

        if (player == owningPlayer()) {
            // Plague Prayers
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 13.0);
            if (!units.empty()) {
                Dice::RollResult result;

                auto prayerRoll = Dice::RollD6();
                if (prayerRoll == 1) {
                    // Failed - take one mortal wound.
                    applyDamage({0, 1}, this);
                } else if (prayerRoll >= 3) {
                    // TODO: Implement area-of-effect Pestilence-pestilence!

                    // Disease-disease!
                    auto numModels = units.front()->remainingModels();
                    Dice::RollD6(numModels, result);
                    units.front()->applyDamage({0, result.rollsGE(6)}, this);
                }
            }
        }
    }

    int PlaguePriest::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
