/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/CovenThrone.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "SoulblightGravelordsPrivate.h"
#include "Lore.h"

namespace Soulblight {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 310;

    struct TableEntry {
        int m_move;
        int m_handmaidenAttacks;
        int m_hostAttacks;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {14, 8, 12},
                    {12, 7, 10},
                    {10, 6, 8},
                    {8,  5, 6},
                    {4,  4, 4}
            };

    bool CovenThrone::s_registered = false;

    Unit *CovenThrone::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_vampireLore[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new CovenThrone(bloodline, lore, trait, artefact, general);
    }

    int CovenThrone::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void CovenThrone::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_vampireLore[0], g_vampireLore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Coven Throne", factoryMethod);
        }
    }

    CovenThrone::CovenThrone(CursedBloodline bloodline, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SoulblightBase(bloodline, "Coven Throne", 14, g_wounds, 10, 4, true, g_pointsPerUnit),
            m_bite(Weapon::Type::Melee, "Predatory Bite", 1, 1, 3, 3, 0, RAND_D3),
            m_stiletto(Weapon::Type::Melee, "Stiletto", 1, 4, 3, 3, -1, 1),
            m_poniards(Weapon::Type::Melee, "Needle-sharp Poniards", 1, 8, 3, 3, 0, 1),
            m_etherealWeapons(Weapon::Type::Melee, "Spectral Claws and Blades", 1, 12, 5, 4, 0, 1) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, MALIGNANT, HERO, WIZARD, COVEN_THRONE};
        m_weapons = {&m_bite, &m_stiletto, &m_poniards, &m_etherealWeapons};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_etherealWeapons.setMount(true);
        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_bite);
        model->addMeleeWeapon(&m_stiletto);
        model->addMeleeWeapon(&m_poniards);
        model->addMeleeWeapon(&m_etherealWeapons);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    void CovenThrone::onWounded() {
        SoulblightBase::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_poniards.setAttacks(g_damageTable[damageIndex].m_handmaidenAttacks);
        m_etherealWeapons.setAttacks(g_damageTable[damageIndex].m_hostAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void CovenThrone::onRestore() {
        SoulblightBase::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    size_t CovenThrone::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds CovenThrone::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll >= 6) && (weapon->name() == m_etherealWeapons.name())) return {0, 1};
        return SoulblightBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void CovenThrone::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(RAND_D3);

        SoulblightBase::onEndCombat(player);
    }

} // namespace Soulblight
