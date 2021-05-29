/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/LordKroak.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Roster.h>
#include <Board.h>
#include "SeraphonPrivate.h"
#include "SeraphonLore.h"

namespace Seraphon {

    class CelestialDeliverance : public Spell {
    public:
        explicit CelestialDeliverance(Unit *caster, int castingValue) :
                Spell(caster, "Celestial Deliverance", castingValue, 10) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::AreaOfEffectDamage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            return apply(castingRoll, unmodifiedCastingRoll, 0.0, 0.0);
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x, double y) override {

            auto units = Board::Instance()->getUnitsWithin(m_caster, GetEnemyId(m_caster->owningPlayer()), m_range);
            auto unitsTargeted = 0;
            for (auto unit : units) {

                if (Dice::RollD6() >= 2) {
                    Wounds wounds{0, Dice::RollD3(), Wounds::Source::Spell};
                    if (unit->hasKeyword(CHAOS) && unit->hasKeyword(DAEMON)) {
                        wounds.mortal = 3;
                    }
                    unit->applyDamage(wounds, m_caster);
                }
                unitsTargeted++;
                if (unitsTargeted > 3) break;
            }
            return Spell::Result::Success;
        }
    };

    static const int g_basesize = 50;
    static const int g_wounds = 18;
    static const int g_pointsPerUnit = 430;

    bool LordKroak::s_registered = false;

    LordKroak::LordKroak(WayOfTheSeraphon way, Constellation constellation, bool isGeneral) :
            SeraphonBase("Lord Kroak", 5, g_wounds, 9, 4, true, g_pointsPerUnit) {
        m_keywords = {ORDER, SERAPHON, SLANN, HERO, WIZARD, STARMASTER, LORD_KROAK};
        m_weapons = {&m_barrier};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 4;
        m_totalUnbinds = 4;

        setWayOfTheSeraphon(way, constellation);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_barrier);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<CelestialDeliverance>(this, 7));
        m_knownSpells.push_back(std::make_unique<CelestialDeliverance>(this, 8));
        m_knownSpells.push_back(std::make_unique<CelestialDeliverance>(this, 9));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Celestial_Apotheosis, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Walk_Between_Realms, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Mystical_Unforging, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Celestial_Equilibrium, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Stellar_Tempest, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Drain_Magic, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateCometsCall(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_commandAbilities.push_back(std::unique_ptr<CommandAbility>(CreateGiftFromTheHeavens(this)));
    }

    Unit *LordKroak::Create(const ParameterList &parameters) {
        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new LordKroak(way, constellation, general);
    }

    void LordKroak::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Lord Kroak", factoryMethod);
        }
    }

    int LordKroak::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = SeraphonBase::extraAttacks(attackingModel, weapon, target);

        // Azyrite Force Barrier
        if (target) {
            extra += target->numModelsWithin(attackingModel, 3.0);
            if (target->hasKeyword(MONSTER)) {
                extra *= 5;
            }
        }

        return extra;
    }

    Wounds LordKroak::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Selfless Protector (Saurus Guard and Saurus Eternity Warden abilities)
        auto guards = Board::Instance()->getUnitsWithin(this, owningPlayer(), 3.0);
        for (auto guard : guards) {
            if (guard->hasKeyword(SAURUS_GUARD) || guard->hasKeyword(ETERNITY_WARDEN)) {
                if (guard->remainingModels() > 0) {
                    if (Dice::RollD6() >= 2) {
                        guard->applyDamage(wounds, this);
                        return {0, 0};
                    }
                }
            }
        }
        return wounds;
    }

    void LordKroak::onStartHero(PlayerId player) {
        SeraphonBase::onStartHero(player);

        // Impeccable Foresight
        if (owningPlayer() == player) {
            Dice::RollResult result;
            Dice::RollD6(3, result);
            m_roster->addCommandPoints(result.rollsGE(4));
        }
    }

    int LordKroak::castingModifier() const {
        auto mod = SeraphonBase::castingModifier();
        // Supreme Master of Order
        return mod + 2;
    }

    int LordKroak::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    int LordKroak::unbindingModifier() const {
        auto mod = SeraphonBase::unbindingModifier();
        // Supreme Master of Order
        return mod + 2;
    }

} //namespace Seraphon
