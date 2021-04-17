/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <gloomspitegitz/Mollog.h>
#include <UnitFactory.h>
#include <Board.h>

namespace GloomspiteGitz {
    static const int g_basesize = 50;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 170;

    bool Mollog::s_registered = false;

    Unit *Mollog::Create(const ParameterList &parameters) {
        auto unit = new Mollog();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure();
        return unit;
    }

    void Mollog::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };

            s_registered = UnitFactory::Register("Mollog", factoryMethod);
        }
    }

    int Mollog::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    Mollog::Mollog() :
            GloomspiteGitzBase("Mollog", 6, g_wounds, 7, 4, false),
            m_jabbertoad(Weapon::Type::Missile, "Jabbertoad", 12, 1, 4, 4, 0, 1),
            m_club(Weapon::Type::Melee, "Puff-fungus Club", 1, 2, 0, 0, 0, 0) {
        m_keywords = {DESTRUCTION, TROGGOTH, GLOOMSPITE_GITZ, DANKHOLD, HERO, MOLLOG};
        m_weapons = {&m_jabbertoad, &m_club};
        m_battleFieldRole = Role::Leader;

        s_globalBraveryMod.connect(this, &Mollog::reassuringPresence, &m_connection);
    }

    Mollog::~Mollog() {
        m_connection.disconnect();
    }

    void Mollog::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_jabbertoad);
        model->addMeleeWeapon(&m_club);
        addModel(model);

        m_points = ComputePoints(1);
    }

    void Mollog::onStartHero(PlayerId player) {
        if (player == owningPlayer()) {
            if (remainingWounds() < g_wounds && remainingWounds() > 0) {
                // Regeneration - heal D3
                // Troggoth Renewal
                if (Dice::RollD6() >= 4 || (inLightOfTheBadMoon() && (Dice::RollD6() >= 4))) {
                    int woundsHealed = Dice::RollD3();
                    if (inLightOfTheBadMoon())
                        woundsHealed *= 2;
                    for (auto &m : m_models) {
                        m->applyHealing(woundsHealed);
                    }
                }
            }
        }
    }

    int Mollog::reassuringPresence(const Unit *unit) {
        // Reassuring Presence
        if (unit->hasKeyword(GLOOMSPITE_GITZ) && isFriendly(unit) &&
            (distanceTo(unit) <= 12.0)) {
            return 1;
        }
        return 0;
    }

    Wounds Mollog::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {

        // Magical Resistance
        if (wounds.source == Wounds::Source::Spell) {
            if (Dice::RollD6() >= 4) {
                return {0, 0, Wounds::Source::Spell};
            }
        }

        auto totalWounds = Unit::applyWoundSave(wounds, attackingUnit);

        // Loyal to the End
        if (m_batSquig) {
            if (totalWounds.mortal > 0) {
                totalWounds.mortal--;
                m_batSquig = false;
            } else if (totalWounds.normal > 0) {
                totalWounds.normal--;
                m_batSquig = false;
            }
        }
        if (m_spiteshroom) {
            if (totalWounds.mortal > 0) {
                totalWounds.mortal--;
                m_spiteshroom = false;
            } else if (totalWounds.normal > 0) {
                totalWounds.normal--;
                m_spiteshroom = false;
            }
        }
        if (m_stalagsquig) {
            const auto roll = Dice::RollD6();
            if (totalWounds.mortal > 0) {
                totalWounds.mortal--;
                if (roll < 5)
                    m_stalagsquig = false;
            } else if (totalWounds.normal > 0) {
                totalWounds.normal--;
                if (roll < 5)
                    m_stalagsquig = false;
            }
        }

        return totalWounds;
    }

    void Mollog::onRestore() {
        Unit::onRestore();

        m_batSquig = true;
        m_spiteshroom = true;
        m_stalagsquig = true;
    }

    void Mollog::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        if (owningPlayer() == player) {
            if (m_batSquig) {
                auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
                if (unit && (distanceTo(unit) <= 18.0)) {
                    if (Dice::RollD6() >= 5) {
                        unit->applyDamage({0, 1}, this);
                    }
                }
            }
        }
    }

    void Mollog::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        if (m_spiteshroom) {
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) <= 3.0)) {
                if (Dice::RollD6() >= 5) {
                    unit->buffModifier(Attribute::To_Hit_Melee, -1, {GamePhase::Combat, m_battleRound, player});
                }
            }
        }
    }

} // namespace GloomspiteGitz
