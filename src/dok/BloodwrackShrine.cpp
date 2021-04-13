/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/BloodwrackShrine.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include "DaughterOfKhainePrivate.h"
#include "LoreOfShadows.h"
#include "DoKCommands.h"

namespace DaughtersOfKhaine {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 13;
    static const int g_pointsPerUnit = 160;

    struct TableEntry {
        int m_move;
        int m_staveAttacks;
        int m_auraOfAgony;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 5, 8, 10, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {6, 6, 2},
                    {5, 5, 2},
                    {4, 4, 3},
                    {3, 3, 4},
                    {2, 2, 5}
            };

    bool BloodwrackShrine::s_registered = false;

    BloodwrackShrine::BloodwrackShrine() :
            DaughterOfKhaine("Bloodwrack Shrine", 6, g_wounds, 8, 5, false),
            m_bloodwrackStare(Weapon::Type::Missile, "Bloodwrack Stare", 10, 1, 0, 0, -7, 0),
            m_whisperclaw(Weapon::Type::Melee, "Whisperclaw", 1, 4, 4, 3, 0, 1),
            m_tailOfSerpents(Weapon::Type::Melee, "Tail of Serpents", 2, RAND_D6, 4, 4, 0, 1),
            m_bloodwrackSpear(Weapon::Type::Melee, "Bloodwrack Spear", 2, 2, 3, 3, -1, RAND_D3),
            m_goadstaves(Weapon::Type::Melee, "Shrinekeeper's Goadstaves", 2, 6, 3, 3, 0, 1) {
        m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, MELUSAI, HERO, WIZARD, BLOODWRACK_MEDUSA, BLOODWRACK_SHRINE};
        m_weapons = {&m_bloodwrackStare, &m_whisperclaw, &m_tailOfSerpents, &m_bloodwrackSpear, &m_goadstaves};
        m_battleFieldRole = Role::Leader_Behemoth;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    void BloodwrackShrine::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_bloodwrackStare);
        model->addMeleeWeapon(&m_whisperclaw);
        model->addMeleeWeapon(&m_tailOfSerpents);
        model->addMeleeWeapon(&m_bloodwrackSpear);
        model->addMeleeWeapon(&m_goadstaves);
        addModel(model);

        m_knownSpells.push_back(
                std::make_unique<BuffModifierSpell>(this, "Enfeebling Foe", 5, 18, Attribute::To_Wound_Melee, -1,
                                                    Abilities::Target::Enemy));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        configureCommon();

        m_points = g_pointsPerUnit;
    }

    Unit *BloodwrackShrine::Create(const ParameterList &parameters) {
        auto unit = new BloodwrackShrine();

        auto temple = (Temple) GetEnumParam("Temple", parameters, g_temple[0]);
        unit->setTemple(temple);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_medusaCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_wizardArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        unit->configure(lore);
        return unit;
    }

    void BloodwrackShrine::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    DaughterOfKhaine::ValueToString,
                    DaughterOfKhaine::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Temple", g_temple[0], g_temple),
                            EnumParameter("Command Trait", g_medusaCommandTraits[0], g_medusaCommandTraits),
                            EnumParameter("Artefact", g_wizardArtefacts[0], g_wizardArtefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Bloodwrack Shrine", factoryMethod);
        }
    }

    void BloodwrackShrine::onWounded() {
        DaughterOfKhaine::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_goadstaves.setAttacks(g_damageTable[damageIndex].m_staveAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void BloodwrackShrine::onRestore() {
        DaughterOfKhaine::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    size_t BloodwrackShrine::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void BloodwrackShrine::onCharged() {
        DaughterOfKhaine::onCharged();

        // Bladed Impact
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && (distanceTo(unit) <= 1.0)) {
            if (Dice::RollD6() >= 2) unit->applyDamage({0, Dice::RollD3()}, this);
        }
    }

    void BloodwrackShrine::onStartHero(PlayerId player) {
        DaughterOfKhaine::onStartHero(player);

        // Aura of Agony
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 7.0);
        const int auraToHit = g_damageTable[getDamageTableIndex()].m_auraOfAgony;
        for (auto unit : units) {
            if (Dice::RollD6() >= auraToHit) {
                unit->applyDamage({0, Dice::RollD3()}, this);
            }
        }
    }

    Wounds BloodwrackShrine::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        if (weapon->name() == m_bloodwrackStare.name()) {
            Dice::RollResult result;
            Dice::RollD6(target->remainingModels(), result);
            return {0, result.rollsGE(5)};
        }
        return DaughterOfKhaine::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int BloodwrackShrine::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace DaughtersOfKhaine

