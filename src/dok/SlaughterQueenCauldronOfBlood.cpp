/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/SlaughterQueenCauldronOfBlood.h>
#include <UnitFactory.h>
#include <Board.h>
#include "DaughterOfKhainePrivate.h"

namespace DaughtersOfKhaine {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 13;
    static const int POINTS_PER_UNIT = 0;

    struct TableEntry {
        int m_move;
        int m_knifeAttacks;
        double m_bloodshield;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 5, 8, 10, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {6, 8, 18},
                    {5, 7, 14},
                    {4, 6, 10},
                    {3, 5, 6},
                    {2, 4, 2}
            };

    bool SlaughterQueenOnCauldronOfBlood::s_registered = false;

    SlaughterQueenOnCauldronOfBlood::SlaughterQueenOnCauldronOfBlood() :
            DaughterOfKhaine("Slaughter Queen on Cauldron of Blood", 6, WOUNDS, 9, 5, false),
            m_burningBlood(Weapon::Type::Missile, "Torrent of Burning Blood", 10, 6, 3, 3, -1, 1),
            m_knives(Weapon::Type::Melee, "Witch Aelves' Sacrificial Knives", 1, 8, 3, 4, 0, 1),
            m_blade(Weapon::Type::Melee, "Slaughter Queen's Blade of Khaine", 1, 4, 3, 4, -1, 1),
            m_deathsword(Weapon::Type::Melee, "Slaughter Queen's Deathsword", 1, 3, 3, 3, -1, RAND_D3),
            m_sword(Weapon::Type::Melee, "Avatar of Khaine's Sword", 2, 4, 3, 3, -2, 3) {
        m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, TOTEM, HERO, PRIEST, WITCH_AELVES, SLAUGHTER_QUEEN,
                      AVATAR_OF_KHAINE, CAULDRON_OF_BLOOD};
        m_weapons = {&m_burningBlood, &m_knives, &m_blade, &m_deathsword, &m_sword};

        // Pact of Blood
        m_totalUnbinds = 1;

        s_globalBraveryMod.connect(this, &SlaughterQueenOnCauldronOfBlood::idolOfWorship, &m_idolSlot);
        s_globalSaveMod.connect(this, &SlaughterQueenOnCauldronOfBlood::bloodShield, &m_bloodshieldSlot);
    }

    SlaughterQueenOnCauldronOfBlood::~SlaughterQueenOnCauldronOfBlood() {
        m_idolSlot.disconnect();
        m_bloodshieldSlot.disconnect();
    }

    bool SlaughterQueenOnCauldronOfBlood::configure(Prayer prayer) {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_burningBlood);
        model->addMeleeWeapon(&m_knives);
        model->addMeleeWeapon(&m_blade);
        model->addMeleeWeapon(&m_deathsword);
        model->addMeleeWeapon(&m_sword);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *SlaughterQueenOnCauldronOfBlood::Create(const ParameterList &parameters) {
        auto unit = new SlaughterQueenOnCauldronOfBlood();

        auto temple = (Temple)GetEnumParam("Temple", parameters, g_temple[0]);
        unit->setTemple(temple);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_priestArtefacts[0]);
        unit->setArtefact(artefact);

        auto prayer = (Prayer) GetEnumParam("Prayer", parameters, g_prayers[0]);

        bool ok = unit->configure(prayer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SlaughterQueenOnCauldronOfBlood::Init() {
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
                            EnumParameter("Prayer", g_prayers[0], g_prayers)
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Slaughter Queen on Cauldron of Blood", factoryMethod);
        }
    }

    void SlaughterQueenOnCauldronOfBlood::onWounded() {
        Unit::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_knives.setAttacks(g_damageTable[damageIndex].m_knifeAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void SlaughterQueenOnCauldronOfBlood::onRestore() {
        Unit::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    int SlaughterQueenOnCauldronOfBlood::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void SlaughterQueenOnCauldronOfBlood::onCharged() {
        Unit::onCharged();

        // Bladed Impact
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && (distanceTo(unit) <= 1.0)) {
            if (Dice::rollD6() >= 2) unit->applyDamage({0, Dice::rollD3()});
        }
    }

    int SlaughterQueenOnCauldronOfBlood::idolOfWorship(const Unit *unit) {
        // Idol of Worship
        if (unit->hasKeyword(DAUGHTERS_OF_KHAINE) && (distanceTo(unit) <= 7.0)) return 1;

        return 0;
    }

    int SlaughterQueenOnCauldronOfBlood::bloodShield(const Unit *target, const Weapon * /*weapon*/) {
        if (isFriendly(target) && target->hasKeyword(DAUGHTERS_OF_KHAINE) &&
            (distanceTo(target) <= g_damageTable[getDamageTableIndex()].m_bloodshield)) {
            return 1;
        }
        return 0;
    }

    int SlaughterQueenOnCauldronOfBlood::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void SlaughterQueenOnCauldronOfBlood::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        m_blade.setDamage(1);

        // Priestess of Khaine
        const auto roll = Dice::rollD6();
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && distanceTo(unit) <= 3.0) {
            // Touch of Death
            if (roll == 1) {
                applyDamage({0, 1});
            } else if (roll >= 3) {
                if (Dice::rollD6() >= 4) {
                    unit->applyDamage({0, Dice::rollD3()});
                }
            }
        } else {
            // Rune of Khorne
            if (roll == 1) {
                applyDamage({0, 1});
            } else if (roll >= 3) {
                m_blade.setDamage(RAND_D3);
            }
        }
    }

} //namespace DaughtersOfKhaine

