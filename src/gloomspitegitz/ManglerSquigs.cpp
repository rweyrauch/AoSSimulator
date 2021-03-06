/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/ManglerSquigs.h>
#include <UnitFactory.h>
#include <Board.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {
    static const int g_basesize = 80;
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 240;

    bool ManglerSquigs::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_gobsToHit;
        int m_ballsAndChainsAttack;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {RAND_3D6, 3, 7},
                    {RAND_2D6, 4, 6},
                    {RAND_D6,  5, 5},
                    {RAND_2D6, 4, 6},
                    {RAND_3D6, 3, 7}
            };

    ManglerSquigs::ManglerSquigs(Allegiance allegiance) :
            GloomspiteGitzBase(allegiance, "Mangler Squigs", RAND_3D6, g_wounds, 10, 4, true, g_pointsPerUnit),
            m_hugeFangFilledGob(Weapon::Type::Melee, "Huge Fang-filled Gobs", 2, 4, 3, 3, -1, RAND_D6),
            m_ballsAndChains(Weapon::Type::Melee, "Balls and Chains", 2, 7, 3, 3, -2, RAND_D3),
            m_grotsBashinStikk(Weapon::Type::Melee, "Grots' Bashin' Stikks", 1, 4, 4, 4, 0, 1) {
        m_keywords = {DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, MONSTER, HERO, MANGLER_SQUIG};
        m_weapons = {&m_hugeFangFilledGob, &m_ballsAndChains, &m_grotsBashinStikk};
        m_battleFieldRole = Role::Behemoth;
        m_hasMount = true;
        m_hugeFangFilledGob.setMount(true);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_hugeFangFilledGob);
        model->addMeleeWeapon(&m_ballsAndChains);
        model->addMeleeWeapon(&m_grotsBashinStikk);
        addModel(model);
    }

    void ManglerSquigs::onRestore() {
        // Reset table-driven attributes
        onWounded();
    }

    int ManglerSquigs::toHitModifier(const Weapon *weapon, const Unit *unit) const {
        int modifier = GloomspiteGitzBase::toHitModifier(weapon, unit);

        // Ker-splat!
        if (m_charged) {
            modifier++;
        }
        return modifier;
    }

    void ManglerSquigs::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_hugeFangFilledGob.setToHit(g_damageTable[damageIndex].m_gobsToHit);
        m_ballsAndChains.setAttacks(g_damageTable[damageIndex].m_ballsAndChainsAttack);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    size_t ManglerSquigs::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }


    Unit *ManglerSquigs::Create(const ParameterList &parameters) {
        auto allegiance = (Allegiance) GetEnumParam("Allegiance", parameters, g_allegiance[0]);
        return new ManglerSquigs(allegiance);
    }

    void ManglerSquigs::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ManglerSquigs::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    ManglerSquigs::ComputePoints,
                    {
                            EnumParameter("Allegiance", g_allegiance[0], g_allegiance),
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Mangler Squigs", factoryMethod);
        }
    }

    void ManglerSquigs::onFriendlyUnitSlain(const Unit *attacker) {
        // Watch Out!
        // get all units within 6" (friend and foe)
        auto units = Board::Instance()->getUnitsWithin(this, PlayerId::None, 6.0);
        for (auto ip : units) {
            int roll = Dice::RollD6();
            if (roll >= 4) {
                int mortalWounds = Dice::RollD3();
                ip->applyDamage({0, mortalWounds}, this);
            }
        }

        GloomspiteGitzBase::onFriendlyUnitSlain(nullptr);
    }

    int ManglerSquigs::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace GloomspiteGitz
