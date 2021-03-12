/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SkinkStarseer.h>
#include <UnitFactory.h>
#include <Roster.h>
#include <spells/MysticShield.h>
#include "SeraphonPrivate.h"
#include "SeraphonLore.h"

namespace Seraphon {

    class ControlFate : public Spell {
    public:
        explicit ControlFate(Unit *caster) :
                Spell(caster, "Control Fate", 7, 18) {
            m_allowedTargets = Abilities::Target::Any;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Result::Failed;

            if (m_caster->isFriendly(target)) {
                if (target->hasKeyword(SERAPHON)) {
                    target->buffModifier(Attribute::To_Save_Melee, 1, defaultDuration());
                    target->buffModifier(Attribute::To_Save_Missile, 1, defaultDuration());
                }
            } else {
                target->buffModifier(Attribute::To_Save_Melee, -1, defaultDuration());
                target->buffModifier(Attribute::To_Save_Missile, -1, defaultDuration());
            }
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }
    };

    static const int g_basesize = 50;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 140;

    bool SkinkStarseer::s_registered = false;

    SkinkStarseer::SkinkStarseer() :
            SeraphonBase("Skink Starseer", 5, g_wounds, 6, 5, true),
            m_astralBolt(Weapon::Type::Missile, "Astral Bolt", 18, 2, 3, 3, -1, RAND_D3),
            m_staff(Weapon::Type::Melee, "Astromancer's Staff", 2, 2, 4, 3, -1, RAND_D3) {
        m_keywords = {ORDER, SERAPHON, SKINK, HERO, WIZARD, STARSEER};
        m_weapons = {&m_astralBolt, &m_staff};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool SkinkStarseer::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_astralBolt);
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<ControlFate>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = ComputePoints(1);

        return true;
    }

    Unit *SkinkStarseer::Create(const ParameterList &parameters) {
        auto unit = new SkinkStarseer();

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        unit->setWayOfTheSeraphon(way, constellation);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfCelestialManipulation[0]);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_skinkCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_vestmentsOfThePriesthood[0]);

        unit->setArtefact(artefact);
        unit->setCommandTrait(trait);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SkinkStarseer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation),
                            EnumParameter("Command Trait", g_skinkCommandTrait[0], g_skinkCommandTrait),
                            EnumParameter("Artefact", g_vestmentsOfThePriesthood[0], g_vestmentsOfThePriesthood),
                            EnumParameter("Lore", g_loreOfCelestialManipulation[0], g_loreOfCelestialManipulation),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Skink Starseer", factoryMethod);
        }
    }

    void SkinkStarseer::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        // Cosmic Herald
        if (owningPlayer() == player) {
            Dice::RollResult result;
            Dice::RollD6(1, result);
            m_roster->addCommandPoints(result.rollsGE(4));
        }
    }

    int SkinkStarseer::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Seraphon
