/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/PrinceVhordrai.h>
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "LegionOfNagashPrivate.h"

namespace Death {
    static const int BASESIZE = 130;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 460;

    struct TableEntry {
        int m_move;
        int m_breath;
        int m_clawAttack;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {14, 6,       7},
                    {12, RAND_D6, 6},
                    {10, 3,       5},
                    {8,  RAND_D3, 4},
                    {6,  1,       3}
            };

    bool PrinceVhordrai::s_registered = false;

    Unit *PrinceVhordrai::Create(const ParameterList &parameters) {
        auto unit = new PrinceVhordrai();

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        unit->setLegion(legion);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_vampireLore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int PrinceVhordrai::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void PrinceVhordrai::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Legion", g_legions[0], g_legions),
                            EnumParameter("Lore", g_vampireLore[0], g_vampireLore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT}
            };
            s_registered = UnitFactory::Register("Prince Vhordrai", factoryMethod);
        }
    }

    PrinceVhordrai::PrinceVhordrai() :
            LegionOfNagashBase("Prince Vhordrai", 14, WOUNDS, 10, 3, true),
            m_bloodlance(Weapon::Type::Melee, "Bloodlance", 2, 4, 3, 3, -2, 2),
            m_maw(Weapon::Type::Melee, "Shordemaire's Maw", 3, 3, 4, 3, -2, RAND_D6),
            m_claws(Weapon::Type::Melee, "Shordemaire's Sword-like Claws", 2, 7, 4, 3, -1, 2) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, ZOMBIE_DRAGON, MONSTER, HERO, WIZARD, PRINCE_VHORDRAI};
        m_weapons = {&m_bloodlance, &m_maw, &m_claws};
        m_battleFieldRole = LeaderBehemoth;
        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool PrinceVhordrai::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_bloodlance);
        model->addMeleeWeapon(&m_maw);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void PrinceVhordrai::onWounded() {
        Unit::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttack);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void PrinceVhordrai::onRestore() {
        Unit::onRestore();

        m_usedChaliceOfBlood = false;

        // Restore table-driven attributes
        onWounded();
    }

    int PrinceVhordrai::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds PrinceVhordrai::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Bloodlance Charge
        if (m_charged && (weapon->name() == m_bloodlance.name())) return {3, 0};
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void PrinceVhordrai::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) {
            // Chalice of Blood
            if (!m_usedChaliceOfBlood && remainingWounds() < wounds()) {
                heal(Dice::rollD6());
                m_usedChaliceOfBlood = true;
            }
        }
    }

    void PrinceVhordrai::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        if (owningPlayer() == player) {
            // Breath of Shyish
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && distanceTo(unit) < 8.0) {
                unit->applyDamage({0, Dice::rollSpecial(g_damageTable[getDamageTableIndex()].m_breath)});
            }
        }
    }

    Wounds PrinceVhordrai::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(1);

        return Unit::onEndCombat(player);
    }

} // namespace Death
