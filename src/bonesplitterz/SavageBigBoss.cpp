/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/SavageBigBoss.h"
#include "BonesplitterzPrivate.h"

namespace Bonesplitterz {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 90;

    bool SavageBigBoss::s_registered = false;

    Unit *SavageBigBoss::Create(const ParameterList &parameters) {
        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_prophetBossCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_heroArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new SavageBigBoss(warclan, trait, artefact, general);
    }

    void SavageBigBoss::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Bonesplitterz::ValueToString,
                    Bonesplitterz::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                            EnumParameter("Command Trait", g_prophetBossCommandTraits[0], g_prophetBossCommandTraits),
                            EnumParameter("Artefact", g_heroArtefacts[0], g_heroArtefacts),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {BONESPLITTERZ}
            };

            s_registered = UnitFactory::Register("Savage Big Boss", factoryMethod);
        }
    }

    SavageBigBoss::SavageBigBoss(Warclan warclan, CommandTrait trait, Artefact artefact, bool isGeneral) :
            Bonesplitterz(warclan, "Savage Big Boss", 5, g_wounds, 7, 6, false, g_pointsPerUnit),
            m_chompasBoss(Weapon::Type::Melee, "Boss Chompa", 1, 6, 3, 3, -1, 2) {
        m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, HERO, SAVAGE_BIG_BOSS};
        m_weapons = {&m_chompasBoss};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_chompasBoss);
        addModel(model);

        m_commandAbilities.push_back(
                std::make_unique<BuffAbilityCommandAbility>(this, "Savage Attack", 12, 12, GamePhase::Combat,
                                                            Ability::Extra_Hit_On_Value,
                                                            6, Abilities::Target::SelfAndFriendly,
                                                            std::vector<Keyword>{BONESPLITTERZ}));
    }

    int SavageBigBoss::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Bonesplitterz
