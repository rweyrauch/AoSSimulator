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
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit* target) override {
            if (target == nullptr) return Result::Failed;
            target->buffModifier(To_Hit_Melee, -1, defaultDuration());
            target->buffModifier(To_Hit_Missile, -1, defaultDuration());
            target->buffModifier(Target_To_Hit_Melee, 1, defaultDuration());
            return Result::Success;
        }
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x, double y) override { return Result::Failed; }
    };

    static const int g_basesize = 60;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 200;

    bool LadyOlynder::s_registered = false;

    Unit *LadyOlynder::Create(const ParameterList &parameters) {
        auto unit = new LadyOlynder();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void LadyOlynder::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LadyOlynder::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    LadyOlynder::ComputePoints,
                    {
                        EnumParameter("Lore", g_lore[0], g_lore),
                        BoolParameter("General")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Lady Olynder", factoryMethod);
        }
    }

    LadyOlynder::LadyOlynder() :
            Nighthaunt("Lady Olynder", 6, g_wounds, 10, 4, true),
            m_staff(Weapon::Type::Melee, "Staff of Midnight", 2, 3, 3, 3, -2, RAND_D3),
            m_claws(Weapon::Type::Melee, "Banshee Handmaidens' Spectral Claws", 1, 6, 4, 4, 0, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, WIZARD, MORTARCH, LADY_OLYNDER};
        m_weapons = {&m_staff, &m_claws};
        m_battleFieldRole = Leader;
        m_hasMount = true;
        m_claws.setMount(true);
        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    bool LadyOlynder::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<GriefStricken>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = ComputePoints(1);

        return true;
    }

    Wounds LadyOlynder::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll == 6) && (weapon->name() == m_claws.name())) {
            return {0, 1};
        }

        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void LadyOlynder::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

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
        Unit::onStartHero(player);

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
        Unit::onRestore();

        m_graveSandsOfTimeUsed = false;
    }

    int LadyOlynder::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Nighthaunt
