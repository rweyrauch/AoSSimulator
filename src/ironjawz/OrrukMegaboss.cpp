/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/OrrukMegaboss.h>
#include <UnitFactory.h>
#include "IronjawzPrivate.h"

namespace Ironjawz {
    static const int g_basesize = 60;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 140;

    bool OrrukMegaboss::s_registered = false;

    OrrukMegaboss::OrrukMegaboss(Warclan warclan, CommandTrait trait, Artefact artefact, bool isGeneral) :
            Ironjawz(warclan, "Orruk Megaboss", 4, g_wounds, 8, 3, false, g_pointsPerUnit),
            m_bossChoppaAndFist(Weapon::Type::Melee, "Boss Choppa and Rip-toof Fist", 1, 6, 3, 3, -1, 2) {
        m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, HERO, MEGABOSS};
        m_weapons = {&m_bossChoppaAndFist};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_bossChoppaAndFist);
        addModel(model);

        m_commandAbilities.push_back(
                std::make_unique<BuffModifierCommandAbility>(this, "Go on Ladz, Get Stuck In!", 12, 12, GamePhase::Combat,
                                                             Attribute::To_Hit_Melee, 1,
                                                             Abilities::Target::SelfAndFriendly,
                                                             std::vector<Keyword>{IRONJAWZ}));
    }

    Unit *OrrukMegaboss::Create(const ParameterList &parameters) {
        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_bossCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_bossArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new OrrukMegaboss(warclan, trait, artefact, general);
    }

    void OrrukMegaboss::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    OrrukMegaboss::Create,
                    Ironjawz::ValueToString,
                    Ironjawz::EnumStringToInt,
                    OrrukMegaboss::ComputePoints,
                    {
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                            EnumParameter("Command Trait", g_bossCommandTraits[0], g_bossCommandTraits),
                            EnumParameter("Artefact", g_bossArtefacts[0], g_bossArtefacts),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {IRONJAWZ}
            };
            s_registered = UnitFactory::Register("Orruk Megaboss", factoryMethod);
        }
    }

    Wounds OrrukMegaboss::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto wounds = Ironjawz::computeReturnedDamage(weapon, saveRoll);
        // Rip-toof Fist
        if (saveRoll == 6) {
            wounds += {0, 1};
        }
        return wounds;
    }

    void OrrukMegaboss::onStartCombat(PlayerId player) {
        m_modelsSlainAtStartOfCombat = m_currentRecord.m_enemyModelsSlain;

        Ironjawz::onStartCombat(player);
    }

    void OrrukMegaboss::onEndCombat(PlayerId player) {
        // Strength from Victory
        if (m_currentRecord.m_enemyModelsSlain > m_modelsSlainAtStartOfCombat) {
            heal(1);
            m_bossChoppaAndFist.setAttacks(m_bossChoppaAndFist.attacks() + 1);
        }
        Ironjawz::onEndCombat(player);
    }

    int OrrukMegaboss::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Ironjawz