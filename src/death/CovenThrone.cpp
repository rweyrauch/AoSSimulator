/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/CovenThrone.h>
#include <UnitFactory.h>
#include "LegionOfNagashPrivate.h"

namespace Death {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 260;

    struct TableEntry {
        int m_move;
        int m_handmaidenAttacks;
        int m_hostAttacks;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {14, 8, 12},
                    {12, 7, 10},
                    {10, 6, 8},
                    {8,  5, 6},
                    {4,  4, 4}
            };

    bool CovenThrone::s_registered = false;

    Unit *CovenThrone::Create(const ParameterList &parameters) {
        auto unit = new CovenThrone();

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        unit->setLegion(legion);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_vampireLore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int CovenThrone::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void CovenThrone::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Legion", g_legions[0], g_legions),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_vampireLore[0], g_vampireLore),
                    },
                    DEATH,
                    {SOULBLIGHT}
            };
            s_registered = UnitFactory::Register("Coven Throne", factoryMethod);
        }
    }

    CovenThrone::CovenThrone() :
            LegionOfNagashBase("Coven Throne", 14, WOUNDS, 10, 4, true),
            m_bite(Weapon::Type::Melee, "Vampire Queen's Predatory Bite", 1, 1, 3, 4, 0, RAND_D3),
            m_stiletto(Weapon::Type::Melee, "Vampire Queen's Stiletto", 1, 4, 3, 3, -1, 1),
            m_poniards(Weapon::Type::Melee, "Handmaidens' Needle-sharp Poniards", 1, 8, 3, 3, 0, 1),
            m_etherealWeapons(Weapon::Type::Melee, "Spectral Host's Ethereal Weapons", 1, 12, 5, 4, 0, 1) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, MALIGNANT, HERO, WIZARD, COVEN_THRONE};
        m_weapons = {&m_bite, &m_stiletto, &m_poniards, &m_etherealWeapons};
        m_hasMount = true;
    }

    bool CovenThrone::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_bite);
        model->addMeleeWeapon(&m_stiletto);
        model->addMeleeWeapon(&m_poniards);
        model->addMeleeWeapon(&m_etherealWeapons);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void CovenThrone::onWounded() {
        Unit::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_poniards.setAttacks(g_damageTable[damageIndex].m_handmaidenAttacks);
        m_etherealWeapons.setAttacks(g_damageTable[damageIndex].m_hostAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void CovenThrone::onRestore() {
        Unit::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    int CovenThrone::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds CovenThrone::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll >= 6) && (weapon->name() == m_etherealWeapons.name())) return {0, 1};
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Wounds CovenThrone::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(1);

        return Unit::onEndCombat(player);
    }

    void CovenThrone::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) deathlyInvocations(3, 12.0);
    }

} // namespace Death
