/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/Mannfred.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include "SoulblightGravelordsPrivate.h"
#include "Lore.h"

namespace Soulblight {

    class WindOfDeath : public Spell {
    public:
        explicit WindOfDeath(Unit *caster) :
                Spell(caster, "Wind of Death", 7, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }
            if (Dice::RollD6() >= 3) {
                target->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell, this}, m_caster);
            }
            auto units = Board::Instance()->getUnitsWithin(target, target->owningPlayer(), 6.0);
            for (auto unit : units) {
                if (Dice::RollD6() >= 3) {
                    unit->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell, this}, m_caster);
                }
            }
            return Spell::Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override {
            return Result::Failed;
        }
    };

    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 380;

    struct TableEntry {
        int m_move;
        int m_clawAttacks;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 6, 8, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {16, 6},
                    {13, 5},
                    {10, 4},
                    {7,  3},
                    {4,  2}
            };

    bool MannfredMortarchOfNight::s_registered = false;

    Unit *MannfredMortarchOfNight::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_vampireLore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new MannfredMortarchOfNight(lore, general);
    }

    int MannfredMortarchOfNight::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void MannfredMortarchOfNight::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Lore", g_vampireLore[0], g_vampireLore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Mannfred, Mortarch of Night", factoryMethod);
        }
    }

    MannfredMortarchOfNight::MannfredMortarchOfNight(Lore lore, bool isGeneral) :
            SoulblightBase(CursedBloodline::Legion_Of_Night, "Mannfred, Mortarch of Night", 16, g_wounds, 10, 3, true, g_pointsPerUnit) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, DEATHLORDS, LEGION_OF_NIGHT, MONSTER, HERO, WIZARD, MORTARCH, MANNFRED};
        m_weapons = {&m_gheistvor, &m_glaive, &m_ebonClaws, &m_clawsAndDaggers};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_ebonClaws.setMount(true);
        m_totalSpells = 2;
        m_totalUnbinds = 2;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_gheistvor);
        model->addMeleeWeapon(&m_glaive);
        model->addMeleeWeapon(&m_ebonClaws);
        model->addMeleeWeapon(&m_clawsAndDaggers);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<WindOfDeath>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_commandAbilities.push_back(std::make_unique<BuffModifierCommandAbility>(this, "Vigour of Undeath", 12, 12, GamePhase::Hero,
                                                                                  std::vector<std::pair<Attribute, int>>{{Attribute::To_Hit_Melee, 1},
                                                                                                                         {Attribute::To_Wound_Melee, 1},
                                                                                                                         {Attribute::To_Hit_Missile, 1},
                                                                                                                         {Attribute::To_Wound_Missile, 1}},
                                                                                  Abilities::Target::SelfAndFriendly, std::vector<Keyword>{SOULBLIGHT_GRAVELORDS}));
    }

    void MannfredMortarchOfNight::onWounded() {
        SoulblightBase::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_ebonClaws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void MannfredMortarchOfNight::onRestore() {
        SoulblightBase::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    size_t MannfredMortarchOfNight::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds MannfredMortarchOfNight::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target,
                                                 int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll >= 6) && (weapon->name() == m_clawsAndDaggers.name())) return {0, 1};
        return SoulblightBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void MannfredMortarchOfNight::onEndCombat(PlayerId player) {
        // Feaster of Souls
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(RAND_D3);

        SoulblightBase::onEndCombat(player);
    }

    void MannfredMortarchOfNight::onEnemyModelSlainWithWeapon(int numSlain, Unit *enemyUnit, const Weapon *weapon,
                                                              const Wounds &weaponDamage) {
        SoulblightBase::onEnemyModelSlainWithWeapon(numSlain, enemyUnit, weapon, weaponDamage);

        // Sword of Unholy Power
        if (weapon->name() == m_gheistvor.name()) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
            for (auto unit : units) {
                if (unit->hasKeyword(SUMMONABLE) && unit->hasKeyword(SOULBLIGHT_GRAVELORDS)) {
                    unit->buffModifier(Attribute::Attacks_Melee, 1, {GamePhase::Combat, m_battleRound, owningPlayer()});
                }
            }
        }
    }

} // namespace Soulblight
