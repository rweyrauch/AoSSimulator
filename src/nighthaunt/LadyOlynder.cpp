/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "nighthaunt/LadyOlynder.h"
#include "NighthauntPrivate.h"
#include "NighthauntLore.h"

namespace Nighthaunt {

    class GriefStricken : public Spell {
    public:
        explicit GriefStricken(Unit *caster) :
                Spell(caster, "Grief Stricken", 7, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Debuff;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Result::Failed;
            target->buffModifier(Attribute::To_Hit_Melee, -1, defaultDuration());
            target->buffModifier(Attribute::To_Hit_Missile, -1, defaultDuration());
            target->buffModifier(Attribute::Target_To_Hit_Melee, 1, defaultDuration());
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }
    };

    static const int g_basesize = 60;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 200;

    bool LadyOlynder::s_registered = false;

    Unit *LadyOlynder::Create(const ParameterList &parameters) {
        auto procession = (Procession) GetEnumParam("Procession", parameters, g_processions[0]);
        auto general = GetBoolParam("General", parameters, false);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        return new LadyOlynder(procession, lore, general);
    }

    void LadyOlynder::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LadyOlynder::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    LadyOlynder::ComputePoints,
                    {
                            EnumParameter("Procession", g_processions[0], g_processions),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Lady Olynder", factoryMethod);
        }
    }

    LadyOlynder::LadyOlynder(Procession procession, Lore lore, bool isGeneral) :
            Nighthaunt(procession, "Lady Olynder", 6, g_wounds, 10, 4, true, g_pointsPerUnit) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, WIZARD, MORTARCH, LADY_OLYNDER};
        m_weapons = {&m_staff, &m_claws};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_claws.setMount(true);
        m_totalSpells = 2;
        m_totalUnbinds = 2;

        s_globalBattleshockFleeModifier.connect(this, &LadyOlynder::mortarchOfGrief, &m_mortarchOfGriefSlot);

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<GriefStricken>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    LadyOlynder::~LadyOlynder() {
        m_mortarchOfGriefSlot.disconnect();
    }

    Wounds LadyOlynder::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll == 6) && (weapon->name() == m_claws.name())) {
            return {0, 1};
        }
        return Nighthaunt::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void LadyOlynder::onStartShooting(PlayerId player) {
        Nighthaunt::onStartShooting(player);

        // Wail of the Damned
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 10.0);
        for (auto unit : units) {
            auto roll = Dice::Roll2D6();
            if (roll > unit->bravery()) {
                unit->applyDamage({0, Dice::RollD3()}, this);
            }
        }
    }

    void LadyOlynder::onStartHero(PlayerId player) {
        Nighthaunt::onStartHero(player);

        // Lifting the Veil
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 12.0);
        if (!units.empty()) {
            const auto roll = Dice::RollD6();
            if (roll >= 2) {
                const auto numSlain = units[0]->applyDamage({0, roll}, this);
                if (numSlain) {
                    heal(Dice::RollD3());
                }
            }
        }

        if (!m_graveSandsOfTimeUsed) {
            auto unit = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), HERO, 6.0);
            if (unit) {
                unit->applyDamage({0, Dice::RollD6()}, this);
                m_graveSandsOfTimeUsed = true;
            } else if (remainingWounds() < wounds()) {
                heal(Dice::RollD6());
                m_graveSandsOfTimeUsed = true;
            }
        }
    }

    void LadyOlynder::onRestore() {
        Nighthaunt::onRestore();

        m_graveSandsOfTimeUsed = false;
    }

    int LadyOlynder::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    int LadyOlynder::mortarchOfGrief(const Unit *unit, int roll) {
        if (!isFriendly(unit) && (distanceTo(unit) < 12.0)) {
            return 1;
        }
        return 0;
    }

} // namespace Nighthaunt
