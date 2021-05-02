/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/KixiTaka.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon {

    class HeraldOfTheOldOnes : public CommandAbility {
    public:
        explicit HeraldOfTheOldOnes(Unit *source) :
                CommandAbility(source, "Herald of the Old Ones", 18, 18, GamePhase::Hero) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_targetKeywords = {SKINK};
            m_effect = Abilities::EffectType::Buff;
        }

    protected:
        bool apply(Unit *target) override {
            if (target == nullptr) {
                return false;
            }
            target->buffModifier(Attribute::To_Hit_Melee, 1, defaultDuration());
            target->buffModifier(Attribute::To_Hit_Missile, 1, defaultDuration());

            return true;
        }

        bool apply(double x, double y) override { return false; }
    };

    static const int g_basesize = 25;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 225;

    bool KixiTakaTheDiviner::s_registered = false;

    KixiTakaTheDiviner::KixiTakaTheDiviner(bool isGeneral) :
            SeraphonBase("Kixi-Taka The Diviner", 8, g_wounds, 6, 5, false, g_pointsPerUnit) {
        m_keywords = {ORDER, SERAPHON, COALESCED, THUNDER_LIZARD, SKINK, HERO, PRIEST, KIXI_TAKA};
        m_weapons = {&m_starbolt, &m_staff};
        m_battleFieldRole = Role::Leader;

        setWayOfTheSeraphon(WayOfTheSeraphon::Coalesced, Constellation::Thunder_Lizard);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_starbolt);
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_commandAbilities.push_back(std::make_unique<HeraldOfTheOldOnes>(this));
    }

    Unit *KixiTakaTheDiviner::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new KixiTakaTheDiviner(general);
    }

    void KixiTakaTheDiviner::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    ORDER,
                    {SERAPHON}
            };
            s_registered = UnitFactory::Register("Kixi-Taka The Diviner", factoryMethod);
        }
    }

    int KixiTakaTheDiviner::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void KixiTakaTheDiviner::onStartHero(PlayerId player) {
        SeraphonBase::onStartHero(player);

        // Star-stone Staff
        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
            for (auto unit : units) {
                if (unit->hasKeyword(SKINK) && (unit->remainingModels() > 0)) {
                    const int mod = unit->hasKeyword(STARBLOOD_STALKERS) ? 1 : 0;
                    if (Dice::RollD6() + mod >= 3) {
                        const Duration duration = {GamePhase::Hero, m_battleRound + 1, owningPlayer()};
                        unit->buffMovement(MovementRule::Run_And_Shoot, true, duration);
                        unit->buffMovement(MovementRule::Run_And_Charge, true, duration);
                        unit->buffModifier(Attribute::To_Save_Melee, 1, duration);
                        unit->buffModifier(Attribute::To_Save_Missile, 1, duration);
                    }
                }
            }
        }
    }

} //namespace Seraphon
