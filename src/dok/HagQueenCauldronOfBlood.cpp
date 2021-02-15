/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/HagQueenCauldronOfBlood.h>
#include <UnitFactory.h>
#include <Board.h>
#include "DaughterOfKhainePrivate.h"

namespace DaughtersOfKhaine {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 13;
    static const int g_pointsPerUnit = 220;

    struct TableEntry {
        int m_move;
        int m_knifeAttacks;
        double m_bloodshield;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 5, 8, 10, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {6, 8, 18},
                    {5, 7, 14},
                    {4, 6, 10},
                    {3, 5, 6},
                    {2, 4, 2}
            };

    bool HagQueenOnCauldronOfBlood::s_registered = false;

    HagQueenOnCauldronOfBlood::HagQueenOnCauldronOfBlood() :
            DaughterOfKhaine("Hag Queen on Cauldron of Blood", 6, g_wounds, 8, 5, false),
            m_burningBlood(Weapon::Type::Missile, "Torrent of Burning Blood", 10, 6, 3, 3, -1, 1),
            m_knives(Weapon::Type::Melee, "Witch Aelves' Sacrificial Knives", 1, 8, 3, 4, 0, 1),
            m_blade(Weapon::Type::Melee, "Haq Queen's Blade of Khaine", 1, 4, 3, 4, -1, 1),
            m_sword(Weapon::Type::Melee, "Avatar of Khaine's Sword", 2, 4, 3, 3, -2, 3) {
        m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, HERO, PRIEST, WITCH_AELVES, HAG_QUEEN, AVATAR_OF_KHAINE,
                      CAULDRON_OF_BLOOD};
        m_weapons = {&m_burningBlood, &m_knives, &m_blade, &m_sword};
        m_battleFieldRole = Leader_Behemoth;

        s_globalBraveryMod.connect(this, &HagQueenOnCauldronOfBlood::idolOfWorship, &m_idolSlot);
        s_globalSaveMod.connect(this, &HagQueenOnCauldronOfBlood::bloodShield, &m_bloodshieldSlot);
    }

    HagQueenOnCauldronOfBlood::~HagQueenOnCauldronOfBlood() {
        m_idolSlot.disconnect();
        m_bloodshieldSlot.disconnect();
    }

    bool HagQueenOnCauldronOfBlood::configure(Prayer prayer) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_burningBlood);
        model->addMeleeWeapon(&m_knives);
        model->addMeleeWeapon(&m_blade);
        model->addMeleeWeapon(&m_sword);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *HagQueenOnCauldronOfBlood::Create(const ParameterList &parameters) {
        auto unit = new HagQueenOnCauldronOfBlood();

        auto temple = (Temple)GetEnumParam("Temple", parameters, g_temple[0]);
        unit->setTemple(temple);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_priestArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto prayer = (Prayer) GetEnumParam("Prayer", parameters, g_prayers[0]);

        bool ok = unit->configure(prayer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void HagQueenOnCauldronOfBlood::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    DaughterOfKhaine::ValueToString,
                    DaughterOfKhaine::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Temple", g_temple[0], g_temple),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_priestArtefacts[0], g_priestArtefacts),
                            EnumParameter("Prayer", g_prayers[0], g_prayers),
                            BoolParameter("General")
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Hag Queen on Cauldron of Blood", factoryMethod);
        }
    }

    void HagQueenOnCauldronOfBlood::onWounded() {
        Unit::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_knives.setAttacks(g_damageTable[damageIndex].m_knifeAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void HagQueenOnCauldronOfBlood::onRestore() {
        Unit::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    int HagQueenOnCauldronOfBlood::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void HagQueenOnCauldronOfBlood::onCharged() {
        Unit::onCharged();

        // Bladed Impact
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && (distanceTo(unit) <= 1.0)) {
            if (Dice::RollD6() >= 2) unit->applyDamage({0, Dice::RollD3()}, this);
        }
    }

    int HagQueenOnCauldronOfBlood::idolOfWorship(const Unit *unit) {
        // Idol of Worship
        if (unit->hasKeyword(DAUGHTERS_OF_KHAINE) && (distanceTo(unit) <= 7.0)) return 1;

        return 0;
    }

    int HagQueenOnCauldronOfBlood::bloodShield(const Unit *target, const Weapon * /*weapon*/) {
        if (isFriendly(target) && target->hasKeyword(DAUGHTERS_OF_KHAINE) &&
            (distanceTo(target) <= g_damageTable[getDamageTableIndex()].m_bloodshield)) {
            return 1;
        }
        return 0;
    }

    int HagQueenOnCauldronOfBlood::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void HagQueenOnCauldronOfBlood::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        m_blade.setDamage(1);

        // Priestess of Khaine
        const auto roll = Dice::RollD6();
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && distanceTo(unit) <= 3.0) {
            // Touch of Death
            if (roll == 1) {
                applyDamage({0, 1}, this);
            } else if (roll >= 3) {
                if (Dice::RollD6() >= 4) {
                    unit->applyDamage({0, Dice::RollD3()}, this);
                }
            }
        } else {
            // Rune of Khorne
            if (roll == 1) {
                applyDamage({0, 1}, this);
            } else if (roll >= 3) {
                m_blade.setDamage(RAND_D3);
            }
        }
    }

} //namespace DaughtersOfKhaine

