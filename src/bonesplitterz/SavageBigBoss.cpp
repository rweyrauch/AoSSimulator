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
        auto unit = new SavageBigBoss();

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        unit->setWarclan(warclan);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_prophetBossCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_heroArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

    SavageBigBoss::SavageBigBoss() :
            Bonesplitterz("Savage Big Boss", 5, g_wounds, 7, 6, false),
            m_chompasBoss(Weapon::Type::Melee, "Boss Chompa", 1, 6, 3, 3, -1, 2) {
        m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, HERO, SAVAGE_BIG_BOSS};
        m_weapons = {&m_chompasBoss};
        m_battleFieldRole = Role::Leader;
    }

    bool SavageBigBoss::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_chompasBoss);
        addModel(model);

        m_commandAbilities.push_back(
                std::make_unique<BuffAbilityCommandAbility>(this, "Savage Attack", 12, 12, Phase::Combat,
                                                            Ability::Extra_Hit_On_Value,
                                                            6, Abilities::Target::SelfAndFriendly,
                                                            std::vector<Keyword>{BONESPLITTERZ}));

        m_points = g_pointsPerUnit;

        return true;
    }

    int SavageBigBoss::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Bonesplitterz
