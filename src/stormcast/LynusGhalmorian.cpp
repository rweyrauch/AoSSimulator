/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LynusGhalmorian.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <stormcast/StormcastSpells.h>
#include <Board.h>

#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {

    class AmethystGale : public Spell {
    public:
        explicit AmethystGale(Unit *caster);

    protected:

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override;

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }

    };

    AmethystGale::AmethystGale(Unit *caster) :
            Spell(caster, "Amethyst Gale", 6, 12) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result
    AmethystGale::apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) {
        if (target == nullptr) {
            return Spell::Result::Failed;
        }

        target->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, m_caster);
        target->buffModifier(Attribute::To_Hit_Melee, -1, defaultDuration());
        target->buffModifier(Attribute::To_Hit_Missile, -1, defaultDuration());

        return Spell::Result::Success;
    }

    class SombreExemplar : public CommandAbility {
    public:
        explicit SombreExemplar(Unit *source) :
                CommandAbility(source, "Sombre Exemplar", 12, 12, GamePhase::Combat) {
            m_allowedTargets = Abilities::Target::Self;
            m_targetKeywords = {ANVILS_OF_THE_HELDENHAMMER};
            m_effect = Abilities::EffectType::Buff;
        }

    protected:

        bool apply(Unit *target) override {

            m_source->buffModifier(Attribute::To_Hit_Melee, 1, defaultDuration());
            auto units = Board::Instance()->getUnitsWithin(m_source, m_source->owningPlayer(), m_rangeGeneral);
            for (auto unit : units) {
                if ((unit->remainingModels() > 0) && unit->hasKeyword(ANVILS_OF_THE_HELDENHAMMER)) {
                    unit->buffModifier(Attribute::To_Hit_Melee, 1, defaultDuration());
                }
            }
            return true;

        }

        bool apply(double x, double y) override { return false; }
    };

    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 240;

    bool LynusGhalmorianOnGryphcharger::s_registered = false;

    LynusGhalmorianOnGryphcharger::LynusGhalmorianOnGryphcharger(Lore lore, MountTrait trait, bool isGeneral) :
            MountedStormcastEternal(Stormhost::Anvils_Of_The_Heldenhammer, "Lynus Ghalmorian on Gryph-charger", 12, g_wounds, 9, 3, false),
            m_aetherstave(Weapon::Type::Melee, "Aetherstave", 2, 4, 3, 3, -1, RAND_D3),
            m_beakAndClaws(Weapon::Type::Melee, "Razor Beak and Claws", 1, 3, 3, 3, -2, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, GRYPH_CHARGER, STORMCAST_ETERNAL, SACROSANCT, HERO, WIZARD,
                      ANVILS_OF_THE_HELDENHAMMER, LORD_ARCANUM, LYNUS_GHALMORIAN};
        m_weapons = {&m_aetherstave, &m_beakAndClaws};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_beakAndClaws.setMount(true);

        setGeneral(isGeneral);

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        s_globalSaveReroll.connect(this, &LynusGhalmorianOnGryphcharger::shieldOfThePaleKnight, &m_shieldConnection);

        m_mountTrait = trait;

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_aetherstave);
        model->addMeleeWeapon(&m_beakAndClaws);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<AmethystGale>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePrimeElectrids(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_commandAbilities.push_back(std::make_unique<SombreExemplar>(this));

        m_points = g_pointsPerUnit;
    }

    LynusGhalmorianOnGryphcharger::~LynusGhalmorianOnGryphcharger() {
        m_shieldConnection.disconnect();
    }

    Unit *LynusGhalmorianOnGryphcharger::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto trait = (MountTrait) GetEnumParam("Mount Trait", parameters, (int) MountTrait::None);
        auto general = GetBoolParam("General", parameters, false);
        return new LynusGhalmorianOnGryphcharger(lore, trait, general);
    }

    void LynusGhalmorianOnGryphcharger::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Mount Trait", g_gryphChargerMountTrait[0], g_gryphChargerMountTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Lynus Ghalmorian on Gryph-charger", factoryMethod);
        }
    }

    Wounds LynusGhalmorianOnGryphcharger::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll,
                                                       int woundRoll) const {
        // Aethereal Strike
        if ((hitRoll == 6) && (weapon->name() == m_beakAndClaws.name())) {
            return {0, 1};
        }
        return StormcastEternal::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void LynusGhalmorianOnGryphcharger::onStartCombat(PlayerId player) {
        MountedStormcastEternal::onStartCombat(player);
        // Spirit Flask
        if (!m_shatteredFlasks) {
            m_shatteredFlasks = DoSpiritFlasks(this);
        }
    }

    int LynusGhalmorianOnGryphcharger::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    Rerolls LynusGhalmorianOnGryphcharger::shieldOfThePaleKnight(const Unit *attacker, const Weapon *weapon,
                                                                 const Unit *target) {
        if (target->hasKeyword(ANVILS_OF_THE_HELDENHAMMER) && isFriendly(target) && (distanceTo(target) < 12) &&
            weapon->isMissile()) {
            return Rerolls::Ones;
        }
        return Rerolls::None;
    }

} // namespace StormcastEternals