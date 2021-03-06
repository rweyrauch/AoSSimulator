/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/AethericNavigator.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KharadronPrivate.h"

namespace KharadronOverlords {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool AethericNavigator::s_registered = false;

    Unit *AethericNavigator::Create(const ParameterList &parameters) {
        auto port = (Skyport) GetEnumParam("Skyport", parameters, g_skyport[0]);
        auto artycle = (Artycle) GetEnumParam("Artycle", parameters, g_artycles[0]);
        auto amendment = (Amendment) GetEnumParam("Amendment", parameters, g_amendments[0]);
        auto footnote = (Footnote) GetEnumParam("Footnote", parameters, g_footnotes[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_navigatorCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_navigatorArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new AethericNavigator(port, artycle, amendment, footnote, trait, artefact, general);
    }

    void AethericNavigator::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    AethericNavigator::Create,
                    KharadronBase::ValueToString,
                    KharadronBase::EnumStringToInt,
                    AethericNavigator::ComputePoints,
                    {
                            EnumParameter("Skyport", g_skyport[0], g_skyport),
                            EnumParameter("Artycle", g_artycles[0], g_artycles),
                            EnumParameter("Amendment", g_amendments[0], g_amendments),
                            EnumParameter("Footnote", g_footnotes[0], g_footnotes),
                            EnumParameter("Command Trait", g_navigatorCommandTraits[0], g_navigatorCommandTraits),
                            EnumParameter("Artefact", g_navigatorArtefacts[0], g_navigatorArtefacts),
                            BoolParameter("General")
                    },
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };
            s_registered = UnitFactory::Register("Aetheric Navigator", factoryMethod);
        }
    }

    AethericNavigator::AethericNavigator(Skyport port, Artycle artycle, Amendment amendment, Footnote footnote, CommandTrait trait, Artefact artefact, bool isGeneral) :
            KharadronBase(port, artycle, amendment, footnote, "Aetheric Navigator", 4, g_wounds, 7, 3, false, g_pointsPerUnit),
            m_pistol(Weapon::Type::Missile, "Ranging Pistol", 15, 2, 3, 3, -1, 1),
            m_zephyrscope(Weapon::Type::Melee, "Zephyrscope", 1, 2, 3, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, HERO, SKYFARER, MARINE, AETHERIC_NAVIGATOR};
        m_weapons = {&m_pistol, &m_zephyrscope};
        m_battleFieldRole = Role::Leader;

        // Aethersight
        m_totalUnbinds = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_pistol);
        model->addMeleeWeapon(&m_zephyrscope);
        addModel(model);
    }

    int AethericNavigator::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void AethericNavigator::onStartHero(PlayerId player) {
        KharadronBase::onStartHero(player);

        // Aetherstorm
        auto flyers = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 36.0);
        for (auto unit : flyers) {
            if (unit->canFly() && unit->remainingModels() > 0) {
                auto roll = Dice::RollD6();
                if (roll == 6) {
                    unit->applyDamage({0, Dice::RollD3(), Wounds::Source::Ability}, this);
                }
                if (roll >= 3) {
                    unit->buffMovement(MovementRule::Halve_Movement, true,
                                       {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
                }
                break;
            }
        }
    }

} //KharadronOverlords
