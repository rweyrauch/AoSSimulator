/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SkinkStarpriest.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>
#include <spells/MysticShield.h>
#include "SeraphonPrivate.h"
#include "SeraphonLore.h"

namespace Seraphon {

    class BlazingStarlight : public Spell {
    public:
        explicit BlazingStarlight(Unit* caster) :
                Spell(caster, "Blazing Starlight", 6, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Debuff;
        }

    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) override {
            if (target == nullptr) return Result::Failed;

            target->buffModifier(To_Hit_Missile, -1, defaultDuration());
            target->buffModifier(To_Hit_Melee, -1, defaultDuration());

            return Result::Success;
        }
        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override { return Result::Failed; }
    };

    static const int g_basesize = 25;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool SkinkStarpriest::s_registered = false;

    SkinkStarpriest::SkinkStarpriest() :
            SeraphonBase("Skink Starpriest", 8, g_wounds, 6, 5, false),
            m_venombolt(Weapon::Type::Missile, "Venombolt", 18, 2, 3, 3, -1, 1),
            m_staff(Weapon::Type::Melee, "Serpent Staff", 1, 2, 4, 3, -1, 1) {
        m_keywords = {ORDER, SERAPHON, SKINK, HERO, WIZARD, STARPRIEST};
        m_weapons = {&m_venombolt, &m_staff};
        m_battleFieldRole = Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool SkinkStarpriest::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_venombolt);
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<BlazingStarlight>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = ComputePoints(1);

        return true;
    }

    Unit *SkinkStarpriest::Create(const ParameterList &parameters) {
        auto unit = new SkinkStarpriest();

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

    void SkinkStarpriest::Init() {
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

            s_registered = UnitFactory::Register("Skink Starpriest", factoryMethod);
        }
    }

    void SkinkStarpriest::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        // Astral Herald
        if (owningPlayer() == player) {
            Dice::RollResult result;
            Dice::RollD6(1, result);
            m_roster->addCommandPoints(result.rollsGE(5));
        }
    }

    int SkinkStarpriest::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Seraphon
