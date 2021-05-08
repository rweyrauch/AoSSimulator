/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricRunefather.h>
#include <Board.h>
#include <UnitFactory.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {

    class LodgeLeader : public CommandAbility {
    public:
        explicit LodgeLeader(Unit *source) :
                CommandAbility(source, "Lodge Leader", 12, 12, GamePhase::Charge) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:

        bool apply(Unit *target) override {
            auto units = Board::Instance()->getUnitsWithin(m_source, m_source->owningPlayer(), m_rangeGeneral);
            for (auto unit : units) {
                unit->buffModifier(Attribute::Charge_Distance, 1, {GamePhase::Charge, m_round, m_source->owningPlayer()});
            }
            return true;
        }

        bool apply(double x, double y) override { return false; }
    };

    static const int g_basesize = 32;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 100;

    bool AuricRunefather::s_registered = false;

    AuricRunefather::AuricRunefather(Lodge lodge, CommandTrait trait, Artefact artefact, bool isGeneral) :
            Fyreslayer(lodge, "Auric Runefather", 4, g_wounds, 8, 4, false, g_pointsPerUnit),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
            m_grandAxe(Weapon::Type::Melee, "Latchkey Grandaxe", 3, 3, 3, 3, -1, 3) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, AURIC_RUNEFATHER};
        m_weapons = {&m_throwingAxe, &m_grandAxe};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_throwingAxe);
        model->addMeleeWeapon(&m_grandAxe);
        addModel(model);

        m_commandAbilities.push_back(std::make_unique<LodgeLeader>(this));
    }

    Unit *AuricRunefather::Create(const ParameterList &parameters) {
        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_fatherSonTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_heirloomArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new AuricRunefather(lodge, trait, artefact, general);
    }

    void AuricRunefather::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    AuricRunefather::Create,
                    Fyreslayer::ValueToString,
                    Fyreslayer::EnumStringToInt,
                    AuricRunefather::ComputePoints,
                    {
                            EnumParameter("Lodge", g_lodge[0], g_lodge),
                            EnumParameter("Command Trait", g_fatherSonTraits[0], g_fatherSonTraits),
                            EnumParameter("Artefact", g_heirloomArtefacts[0], g_heirloomArtefacts),
                            BoolParameter("General")
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Auric Runefather", factoryMethod);
        }
    }

    int AuricRunefather::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void AuricRunefather::onStartHero(PlayerId player) {
        Fyreslayer::onStartHero(player);

        if (owningPlayer() == player) {
            // Stare Down
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) <= 3.0)) {
                unit->buffModifier(Attribute::Bravery, -Dice::RollD3(),
                                   {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
            }
        }
    }

    void AuricRunefather::onEndCombat(PlayerId player) {
        Fyreslayer::onEndCombat(player);

        // Weapon-breaker
        auto unit = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), HERO, 3.0);
        if (unit) {
            if (Dice::RollD6() == 6) {
                // TODO: buff only affects a single weapon
                unit->buffModifier(Attribute::To_Hit_Melee, -1,
                                   {GamePhase::Battleshock, std::numeric_limits<int>::max(), owningPlayer()});
            }
        }
    }

} // namespace Fyreslayers