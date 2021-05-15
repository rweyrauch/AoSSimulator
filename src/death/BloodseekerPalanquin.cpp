/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/BloodseekerPalangquin.h>
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "LegionOfNagashPrivate.h"
#include "DeathLore.h"

namespace Death {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 300;

    struct TableEntry {
        int m_move;
        double m_wailRange;
        int m_hostAttacks;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {14, 9, 12},
                    {12, 8, 10},
                    {10, 7, 8},
                    {8,  6, 6},
                    {4,  5, 4}
            };

    bool BloodseekerPalanquin::s_registered = false;

    Unit *BloodseekerPalanquin::Create(const ParameterList &parameters) {
        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_vampireLore[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new BloodseekerPalanquin(legion, lore, trait, artefact, general);
    }

    int BloodseekerPalanquin::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void BloodseekerPalanquin::Init() {
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
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT}
            };
            s_registered = UnitFactory::Register("Bloodseeker Palanquin(d)", factoryMethod);
        }
    }

    BloodseekerPalanquin::BloodseekerPalanquin(Legion legion, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            LegionOfNagashBase(legion, "Bloodseeker Palanquin", 14, g_wounds, 10, 4, true, g_pointsPerUnit),
            m_wail(Weapon::Type::Missile, "Wail of the Damned", 9, 1, 0, 0, -7, 0),
            m_blade(Weapon::Type::Melee, "Sanguinarch's Bloodletting Blade", 1, 4, 3, 3, -1, RAND_D3),
            m_etherealWeapons(Weapon::Type::Melee, "Spectral Host's Ethereal Weapons", 1, 12, 5, 4, 0, 1) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, MALIGNANT, HERO, WIZARD, BLOODSEEKER_PALANQUIN};
        m_weapons = {&m_wail, &m_blade, &m_etherealWeapons};
        m_battleFieldRole = Role::Leader_Behemoth;

        m_hasMount = true;
        m_etherealWeapons.setMount(true);

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_wail);
        model->addMeleeWeapon(&m_blade);
        model->addMeleeWeapon(&m_etherealWeapons);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    void BloodseekerPalanquin::onWounded() {
        LegionOfNagashBase::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_etherealWeapons.setAttacks(g_damageTable[damageIndex].m_hostAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void BloodseekerPalanquin::onRestore() {
        LegionOfNagashBase::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    size_t BloodseekerPalanquin::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds BloodseekerPalanquin::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target,
                                              int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll >= 6) && (weapon->name() == m_etherealWeapons.name())) return {0, 1};
        return LegionOfNagashBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void BloodseekerPalanquin::onStartHero(PlayerId player) {
        LegionOfNagashBase::onStartHero(player);

        if (owningPlayer() == player) deathlyInvocations(2, 6.0);
    }

    void BloodseekerPalanquin::onStartShooting(PlayerId player) {
        LegionOfNagashBase::onStartShooting(player);

        // Wail of the Damned
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()),
                                                       g_damageTable[getDamageTableIndex()].m_wailRange);
        for (auto unit : units) {
            if (Dice::Roll2D6() > unit->bravery()) {
                unit->applyDamage({0, Dice::RollD3()}, this);
            }
        }
    }

} // namespace Death
