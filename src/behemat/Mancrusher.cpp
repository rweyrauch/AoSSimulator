/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <behemat/Mancrusher.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SonsOfBehehmetPrivate.h"

namespace SonsOfBehemat {
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 12;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 180;
    static const int POINTS_MAX_UNIT_SIZE = 480;

    struct TableEntry {
        int m_stompingCharge;
        int m_clubAttacks;
        int m_eadbuttDamage;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {2, 10, 4},
                    {3, 9, 3},
                    {4, 8, 3},
                    {5, 6,  2},
                    {6, 4,  1}
            };

    bool Mancrusher::s_registered = false;

    Mancrusher::Mancrusher() :
            SonsOfBehematBase("Mancrusher Gargants", 8, WOUNDS, 7, 5, false),
            m_eadbutt(Weapon::Type::Melee, "'Eadbutt", 1, 1, 4, 3, -3, 4),
            m_club(Weapon::Type::Melee, "Massive Club", 3, 10, 3, 3, -1, 1),
            m_kick(Weapon::Type::Melee, "Mighty Kick", 2, 1, 3, 3, -2, RAND_D3),
            m_rocks(Weapon::Type::Missile, "Chuck Rocks", 18, RAND_D3, 4, 3, -1, RAND_D3) {
        m_weapons = {&m_eadbutt, &m_club, &m_kick, &m_rocks};
        m_battleFieldRole = Behemoth;
        m_keywords = {DESTRUCTION, SONS_OF_BEHEMAT, GARGANT, MONSTER, MANCRUSHER};
    }

    bool Mancrusher::configure(int numModels) {

        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_eadbutt);
            model->addMeleeWeapon(&m_club);
            model->addMeleeWeapon(&m_kick);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    void Mancrusher::onWounded() {
        m_eadbutt.setDamage(g_damageTable[getDamageTableIndex()].m_eadbuttDamage);
        m_club.setAttacks(g_damageTable[getDamageTableIndex()].m_clubAttacks);
    }

    int Mancrusher::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void Mancrusher::onRestore() {
        Unit::onRestore();

        // Reset table-driven attributes
        onWounded();
    }

    Unit *Mancrusher::Create(const ParameterList &parameters) {
        auto unit = new Mancrusher();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto loathing = (FierceLoathing) GetEnumParam("Fierce Loathing", parameters, g_loathings[0]);
        unit->setFierceLoating(loathing);

        auto tribe = (Tribe) GetEnumParam("Tribe", parameters, g_tribe[0]);
        unit->setTribe(tribe);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Mancrusher::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Mancrusher::Create,
                    SonsOfBehematBase::ValueToString,
                    SonsOfBehematBase::EnumStringToInt,
                    Mancrusher::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Fierce Loathing", g_loathings[0], g_loathings),
                            EnumParameter("Tribe", g_tribe[0], g_tribe)
                    },
                    DESTRUCTION,
                    {SONS_OF_BEHEMAT}
            };
            s_registered = UnitFactory::Register("Mancrusher Gargants", factoryMethod);
        }
    }

    int Mancrusher::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    void Mancrusher::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        m_rocks.activate(false);
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 18.0f);
        for (auto unit : units) {
            // Chuck Rocks
            if (unit->isGeneral()) {
                m_rocks.activate();
            }
        }
    }

    Wounds Mancrusher::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto wounds = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);

        // Getting Stuck In
        if (m_tribe == Tribe::Stomper) {
            if (target->remainingModels() >= 20) wounds.normal += 2;
            else if (target->remainingModels() >= 10) wounds.normal += 1;
        }
        return wounds;
    }

} // namespace SonsOfBehemat
