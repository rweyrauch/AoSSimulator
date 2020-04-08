/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/LoonbossOnManglerSquigs.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace GloomspiteGitz {
    static const int BASESIZE = 80;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 280;

    bool LoonbossOnManglerSquigs::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_gobsToHig;
        int m_ballsAndChainsAttack;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {RAND_3D6, 3, 7},
                    {RAND_2D6, 4, 6},
                    {RAND_D6,  5, 5},
                    {RAND_2D6, 4, 6},
                    {RAND_3D6, 3, 7}
            };

    LoonbossOnManglerSquigs::LoonbossOnManglerSquigs() :
            GloomspiteGitzBase("Loonboss on Mangler Squigs", RAND_3D6, WOUNDS, 10, 4, true),
            m_hugeFangFilledGob(Weapon::Type::Melee, "Huge Fang-filled Gobs", 2, 4, 3, 3, -1, RAND_D6),
            m_moonCutta(Weapon::Type::Melee, "Moon-cutta", 1, 5, 3, 4, 0, 1),
            m_ballsAndChains(Weapon::Type::Melee, "Balls and Chains", 2, 7, 3, 3, -2, RAND_D3),
            m_grotsBashinStikk(Weapon::Type::Melee, "Grots' Bashin' Stikks", 1, 4, 4, 4, 0, 1) {
        m_keywords = {DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, MONSTER, HERO, LOONBOSS, MANGLER_SQUIG};
        m_weapons = {&m_hugeFangFilledGob, &m_moonCutta, &m_ballsAndChains, &m_grotsBashinStikk};
    }

    bool LoonbossOnManglerSquigs::configure() {
        auto model = new Model(BASESIZE, wounds());

        model->addMeleeWeapon(&m_moonCutta);
        model->addMeleeWeapon(&m_hugeFangFilledGob);
        model->addMeleeWeapon(&m_ballsAndChains);
        model->addMeleeWeapon(&m_grotsBashinStikk);

        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int LoonbossOnManglerSquigs::toHitModifier(const Weapon *weapon, const Unit *unit) const {
        int modifier = GloomspiteGitzBase::toHitModifier(weapon, unit);

        // Ker-splat!
        if (m_charged) {
            modifier++;
        }

        return modifier;
    }


    void LoonbossOnManglerSquigs::onRestore() {
        // Reset table-driven attributes
        onWounded();
    }


    void LoonbossOnManglerSquigs::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_hugeFangFilledGob.setToHit(g_damageTable[damageIndex].m_gobsToHig);
        m_ballsAndChains.setAttacks(g_damageTable[damageIndex].m_ballsAndChainsAttack);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int LoonbossOnManglerSquigs::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Unit *LoonbossOnManglerSquigs::Create(const ParameterList &parameters) {
        auto unit = new LoonbossOnManglerSquigs();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void LoonbossOnManglerSquigs::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LoonbossOnManglerSquigs::Create,
                    nullptr,
                    nullptr,
                    LoonbossOnManglerSquigs::ComputePoints,
                    {
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Loonboss on Mangler Squigs", factoryMethod);
        }
    }

    void LoonbossOnManglerSquigs::onSlain() {
        // Watch Out!
        // get all units within 6" (friend and foe)
        auto units = Board::Instance()->getUnitsWithin(this, PlayerId::None, 6.0);
        for (auto ip : units) {
            int roll = Dice::rollD6();
            if (roll >= 4) {
                int mortalWounds = Dice::rollD3();
                ip->applyDamage({0, mortalWounds});
            }
        }
        GloomspiteGitzBase::onSlain();
    }

    void LoonbossOnManglerSquigs::onStartHero(PlayerId player) {
        if (player == owningPlayer()) {
            // Redcap Mushrooms
            m_toHitRerolls = NoRerolls;
            m_toWoundRerolls = NoRerolls;

            if (!m_eatenRedcapMushroom) {
                if (m_meleeTarget) {
                    std::cout << "Eating the Redcap Mushroom!" << std::endl;
                    m_eatenRedcapMushroom = true;
                    m_toHitRerolls = RerollFailed;
                    m_toWoundRerolls = RerollFailed;
                }
            }
        }
    }

    Rerolls LoonbossOnManglerSquigs::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (weapon->name() == m_moonCutta.name())
            return m_toHitRerolls;
        return GloomspiteGitzBase::toHitRerolls(weapon, target);
    }

    Rerolls LoonbossOnManglerSquigs::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (weapon->name() == m_moonCutta.name())
            return m_toWoundRerolls;
        return GloomspiteGitzBase::toWoundRerolls(weapon, target);
    }

    int LoonbossOnManglerSquigs::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace GloomspiteGitz
