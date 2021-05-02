/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/LordOfPain.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 150;

    bool LordOfPain::s_registered = false;

    LordOfPain::LordOfPain() :
            SlaaneshBase("Lord of Pain", 6, g_wounds, 7, 4, false, g_pointsPerUnit),
            m_mace(Weapon::Type::Melee, "Soulpiercer Mace", 2, 5, 3, 3, -1, 2) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, HERO, LORD_OF_PAIN};
        m_weapons = {&m_mace};
        m_battleFieldRole = Role::Leader;
    }

    void LordOfPain::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_mace);
        addModel(model);

        m_commandAbilities.push_back(
                std::make_unique<BuffRerollCommandAbility>(this, "Paragon of Depravity", 12, 12, GamePhase::Combat,
                                                           Attribute::To_Hit_Melee, Rerolls::Failed,
                                                           Abilities::Target::SelfAndFriendly,
                                                           std::vector<Keyword>{MORTAL, HEDONITE}));
    }

    Unit *LordOfPain::Create(const ParameterList &parameters) {
        auto unit = new LordOfPain();

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure();
        return unit;
    }

    void LordOfPain::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LordOfPain::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    LordOfPain::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Lord of Pain", factoryMethod);
        }
    }

    int LordOfPain::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    Wounds LordOfPain::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto unsavedWounds = Unit::applyWoundSave(wounds, attackingUnit);

        // Share the Pain
        Dice::RollResult woundSaves, mortalSaves;
        Dice::RollD6(unsavedWounds.normal, woundSaves);
        Dice::RollD6(unsavedWounds.mortal, mortalSaves);

        Wounds totalWounds = unsavedWounds;
        totalWounds.normal -= woundSaves.rollsGE(5);
        totalWounds.mortal -= mortalSaves.rollsGE(5);

        if ((totalWounds.normal < unsavedWounds.normal) ||
            (totalWounds.mortal < unsavedWounds.mortal)) {
            attackingUnit->applyDamage({0, 1}, this);
        }

        return totalWounds;
    }

} // Slannesh
