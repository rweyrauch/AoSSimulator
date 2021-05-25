/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/PrinceVhordrai.h>
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "SoulblightGravelordsPrivate.h"
#include "Lore.h"

namespace Soulblight {
    static const int g_basesize = 130;
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 455;

    struct TableEntry {
        int m_move;
        int m_breath;
        int m_clawAttack;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {14, RAND_D6, 7},
                    {12, RAND_D6, 6},
                    {10, 3,       5},
                    {8,  RAND_D3, 4},
                    {6,  1,       3}
            };

    bool PrinceVhordrai::s_registered = false;

    Unit *PrinceVhordrai::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_vampireLore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new PrinceVhordrai(lore, general);
    }

    int PrinceVhordrai::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void PrinceVhordrai::Init() {
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
            s_registered = UnitFactory::Register("Prince Vhordrai", factoryMethod);
        }
    }

    PrinceVhordrai::PrinceVhordrai(Lore lore, bool isGeneral) :
            SoulblightBase(CursedBloodline::Kastelai_Dynasty, "Prince Vhordrai", 14, g_wounds, 10, 3, true, g_pointsPerUnit),
            m_bloodlance(Weapon::Type::Melee, "Bloodlance", 2, 4, 3, 3, -2, 2),
            m_maw(Weapon::Type::Melee, "Snapping Maw", 3, 3, 4, 3, -2, RAND_D6),
            m_claws(Weapon::Type::Melee, "Sword-like Claws", 2, 7, 4, 3, -1, 2) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, KASTELAI_DYNASTY, ZOMBIE_DRAGON, MONSTER, HERO, WIZARD, PRINCE_VHORDRAI};
        m_weapons = {&m_bloodlance, &m_maw, &m_claws};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_maw.setMount(true);
        m_claws.setMount(true);
        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_bloodlance);
        model->addMeleeWeapon(&m_maw);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<BuffModifierSpell>(this, "Quickblood", 7, 0,
                std::vector<std::pair<Attribute, int>>{{Attribute::To_Hit_Melee, 1}, {Attribute::To_Wound_Melee, 1}},
                Abilities::Target::Self));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    void PrinceVhordrai::onWounded() {
        SoulblightBase::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttack);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void PrinceVhordrai::onRestore() {
        SoulblightBase::onRestore();

        m_usedChaliceOfBlood = false;

        // Restore table-driven attributes
        onWounded();
    }

    size_t PrinceVhordrai::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds PrinceVhordrai::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Bloodlance Charge
        if (m_charged && (weapon->name() == m_bloodlance.name())) return {weapon->damage()+2, 0};
        return SoulblightBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void PrinceVhordrai::onStartHero(PlayerId player) {
        SoulblightBase::onStartHero(player);

        if (owningPlayer() == player) {
            // Chalice of Blood
            if (!m_usedChaliceOfBlood && remainingWounds() < wounds()) {
                heal(Dice::RollD6());
                m_usedChaliceOfBlood = true;
            }
        }
    }

    void PrinceVhordrai::onStartShooting(PlayerId player) {
        SoulblightBase::onStartShooting(player);

        if (owningPlayer() == player) {
            // Breath of Shyish
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && distanceTo(unit) < 9.0) {
                if (Dice::RollD6() >= 3) {
                    unit->applyDamage({0, Dice::RollSpecial(g_damageTable[getDamageTableIndex()].m_breath)}, this);
                }
            }
        }
    }

    void PrinceVhordrai::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(RAND_D3);

        SoulblightBase::onEndCombat(player);
    }

    int PrinceVhordrai::weaponRend(const Model *attackingModel, const Weapon *weapon, const Unit *target, int hitRoll,
                                   int woundRoll) const {
        // Bloodlance Charge
        if (m_charged && (weapon->name() == m_bloodlance.name())) return (weapon->rend()-1);
        return SoulblightBase::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
    }

} // namespace Soulblight
