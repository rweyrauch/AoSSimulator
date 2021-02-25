/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/ColossalSquig.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace GloomspiteGitz {
    static const int g_basesize = 120; // oval
    static const int g_wounds = 16;
    static const int g_pointsPerUnit = 300;

    bool ColossalSquig::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_jawsToHit;
        int m_tramplingAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 7, 10, 13, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {RAND_4D6, 2, 10},
                    {RAND_3D6, 3, 8},
                    {RAND_2D6, 4, 6},
                    {RAND_2D6, 5, 4},
                    {RAND_D6,  6, 2}
            };

    ColossalSquig::ColossalSquig() :
            GloomspiteGitzBase("Colossal Squig", RAND_4D6, g_wounds, 10, 5, false),
            m_puffSpores(Weapon::Type::Missile, "Puff Spores", 8, 1, 5, 5, 0, RAND_D3),
            m_enormousJaws(Weapon::Type::Melee, "Enormous Jaws", 3, 8, 2, 3, -2, RAND_D3),
            m_tramplingFeet(Weapon::Type::Melee, "Trampling Feet", 1, 10, 5, 3, -1, 1) {
        m_keywords = {DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, MONSTER, HERO, COLOSSAL_SQUIG};
        m_weapons = {&m_puffSpores, &m_enormousJaws, &m_tramplingFeet};
        m_battleFieldRole = Behemoth;
    }

    void ColossalSquig::onRestore() {
        GloomspiteGitzBase::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    bool ColossalSquig::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_enormousJaws);
        model->addMeleeWeapon(&m_tramplingFeet);
        model->addMissileWeapon(&m_puffSpores);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    void ColossalSquig::onWounded() {
        GloomspiteGitzBase::onWounded();
        const int damageIndex = getDamageTableIndex();
        m_enormousJaws.setToHit(g_damageTable[damageIndex].m_jawsToHit);
        m_tramplingFeet.setAttacks(g_damageTable[damageIndex].m_tramplingAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void ColossalSquig::onFriendlyUnitSlain() {
        GloomspiteGitzBase::onFriendlyUnitSlain();
        // Fungoid Squig Explosion
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        for (auto unit : units) {
            if (Dice::RollD6() >= 2) {
                unit->applyDamage({0, Dice::RollD3()}, this);
            }
        }
        // TODO: Setup 5 cave squigs w/in 9" of this model and outside of 3" from enemy models.
    }

    Wounds ColossalSquig::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Swallowed Whole
        if ((hitRoll == 6) && (weapon->name() == m_enormousJaws.name())) {
            Wounds wounds = {0, Dice::RollD3()};
            return wounds;
        }
        return GloomspiteGitzBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int ColossalSquig::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }


    Unit *ColossalSquig::Create(const ParameterList &parameters) {
        auto unit = new ColossalSquig();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ColossalSquig::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ColossalSquig::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    ColossalSquig::ComputePoints,
                    {
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Colossal Squig", factoryMethod);
        }
    }

    int ColossalSquig::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = GloomspiteGitzBase::targetHitModifier(weapon, attacker);
        // Puff Spores
        if (!weapon->isMissile()) {
            modifier -= 1;
        }
        return modifier;
    }

    void ColossalSquig::onCharged() {
        GloomspiteGitzBase::onCharged();
        // Crazed Charge
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1);
        for (auto ip : units) {
            if (Dice::RollD6() == 6) {
                ip->applyDamage({0, 1}, this);
            }
        }
    }

    int ColossalSquig::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace GloomspiteGitz
