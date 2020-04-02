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
    static const int BASESIZE = 120; // oval
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 300;

    bool ColossalSquig::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_jawsToHit;
        int m_tramplingAttacks;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 7, 10, 13, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {RAND_4D6, 2, 10},
                    {RAND_3D6, 3, 8},
                    {RAND_2D6, 4, 6},
                    {RAND_2D6, 5, 4},
                    {RAND_D6,  6, 2}
            };

    ColossalSquig::ColossalSquig() :
            GloomspiteGitzBase("Colossal Squig", RAND_4D6, WOUNDS, 10, 5, false),
            m_puffSpores(Weapon::Type::Missile, "Puff Spores", 8, 1, 5, 5, 0, RAND_D3),
            m_enormousJaws(Weapon::Type::Melee, "Enormous Jaws", 3, 8, 2, 3, -2, RAND_D3),
            m_tramplingFeet(Weapon::Type::Melee, "Trampling Feet", 1, 10, 5, 3, -1, 1) {
        m_keywords = {DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, MONSTER, HERO, COLOSSAL_SQUIG};
        m_weapons = {&m_puffSpores, &m_enormousJaws, &m_tramplingFeet};
    }

    void ColossalSquig::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    bool ColossalSquig::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_enormousJaws);
        model->addMeleeWeapon(&m_tramplingFeet);
        model->addMissileWeapon(&m_puffSpores);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void ColossalSquig::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_enormousJaws.setToHit(g_damageTable[damageIndex].m_jawsToHit);
        m_tramplingFeet.setAttacks(g_damageTable[damageIndex].m_tramplingAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void ColossalSquig::onSlain() {
        // Fungoid Squig Explosion
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0f);
        for (auto unit : units) {
            if (Dice::rollD6() >= 2) {
                unit->applyDamage({0, Dice::rollD3()});
            }
        }
        // TODO: Setup 5 cave squigs w/in 9" of this model and outside of 3" from enemy models.
    }

    Wounds ColossalSquig::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Swallowed Whole
        if ((hitRoll == 6) && (weapon->name() == m_enormousJaws.name())) {
            Wounds wounds = {0, Dice::rollD3()};
            return wounds;
        }
        return GloomspiteGitzBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int ColossalSquig::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
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
                    nullptr,
                    nullptr,
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
        // Crazed Charge
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1);
        for (auto ip : units) {
            if (Dice::rollD6() == 6) {
                ip->applyDamage({0, 1});
            }
        }
    }

    int ColossalSquig::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace GloomspiteGitz
