/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Bloodstoker.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 80;

    bool Bloodstoker::s_registered = false;

    Bloodstoker::Bloodstoker(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral) :
            KhorneBase("Bloodstoker", 6, g_wounds, 8, 4, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, BLOODSTOKER};
        m_weapons = {&m_tortureBlade, &m_bloodWhip};
        m_battleFieldRole = Role::Leader;

        setSlaughterHost(host);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_tortureBlade);
        model->addMeleeWeapon(&m_bloodWhip);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *Bloodstoker::Create(const ParameterList &parameters) {
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortalbloodboundCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_mortalArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new Bloodstoker(host, trait, artefact, general);
    }

    void Bloodstoker::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Bloodstoker::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    Bloodstoker::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            EnumParameter("Command Trait", g_mortalbloodboundCommandTraits[0],
                                          g_mortalbloodboundCommandTraits),
                            EnumParameter("Artefact", g_mortalArtefacts[0], g_mortalArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Bloodstoker", factoryMethod);
        }
    }

    int Bloodstoker::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void Bloodstoker::onStartMovement(PlayerId player) {
        KhorneBase::onStartMovement(player);

        if (owningPlayer() == player) {
            // Whipped to Fury
            auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), MORTAL, 8.0);
            if (unit != nullptr) {
                unit->buffModifier(Attribute::Charge_Distance, 3, {GamePhase::Movement, m_battleRound + 1, owningPlayer()});
                unit->buffModifier(Attribute::Run_Distance, 3, {GamePhase::Movement, m_battleRound + 1, owningPlayer()});
                unit->buffReroll(Attribute::To_Wound_Melee, Rerolls::Failed,
                                 {GamePhase::Movement, m_battleRound + 1, owningPlayer()});
                unit->buffReroll(Attribute::To_Wound_Missile, Rerolls::Failed,
                                 {GamePhase::Movement, m_battleRound + 1, owningPlayer()});
            }
        }
    }

} // namespace Khorne