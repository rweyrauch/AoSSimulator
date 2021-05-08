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
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 12;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 3;
    static const int g_pointsPerBlock = 180;
    static const int g_pointsMaxUnitSize = 480;

    struct TableEntry {
        int m_stompingCharge;
        int m_clubAttacks;
        int m_eadbuttDamage;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {2, 10, 4},
                    {3, 9,  3},
                    {4, 8,  3},
                    {5, 6,  2},
                    {6, 4,  1}
            };

    bool Mancrusher::s_registered = false;

    Mancrusher::Mancrusher(Tribe tribe, int numModels, FierceLoathing loathing, int points) :
            SonsOfBehematBase("Mancrusher Gargants", 8, g_wounds, 7, 5, false, points) {
        m_weapons = {&m_eadbutt, &m_club, &m_kick, &m_rocks};
        m_battleFieldRole = Role::Behemoth;
        m_keywords = {DESTRUCTION, SONS_OF_BEHEMAT, GARGANT, MONSTER, MANCRUSHER};

        setFierceLoating(loathing);
        setTribe(tribe);

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_eadbutt);
            model->addMeleeWeapon(&m_club);
            model->addMeleeWeapon(&m_kick);
            addModel(model);
        }
    }

    void Mancrusher::onWounded() {
        m_eadbutt.setDamage(g_damageTable[getDamageTableIndex()].m_eadbuttDamage);
        m_club.setAttacks(g_damageTable[getDamageTableIndex()].m_clubAttacks);
    }

    size_t Mancrusher::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void Mancrusher::onRestore() {
        SonsOfBehematBase::onRestore();

        // Reset table-driven attributes
        onWounded();
    }

    Unit *Mancrusher::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto loathing = (FierceLoathing) GetEnumParam("Fierce Loathing", parameters, g_loathings[0]);
        auto tribe = (Tribe) GetEnumParam("Tribe", parameters, g_tribe[0]);

        return new Mancrusher(tribe, numModels, loathing, ComputePoints(parameters));
    }

    void Mancrusher::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Mancrusher::Create,
                    SonsOfBehematBase::ValueToString,
                    SonsOfBehematBase::EnumStringToInt,
                    Mancrusher::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Fierce Loathing", g_loathings[0], g_loathings),
                            EnumParameter("Tribe", g_tribe[0], g_tribe)
                    },
                    DESTRUCTION,
                    {SONS_OF_BEHEMAT}
            };
            s_registered = UnitFactory::Register("Mancrusher Gargants", factoryMethod);
        }
    }

    int Mancrusher::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void Mancrusher::onStartShooting(PlayerId player) {
        SonsOfBehematBase::onStartShooting(player);

        m_rocks.activate(false);
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 18.0f);
        for (auto unit : units) {
            // Chuck Rocks
            if (unit->isGeneral()) {
                m_rocks.activate();
            }
        }
    }

    Wounds Mancrusher::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto wounds = SonsOfBehematBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);

        // Getting Stuck In
        if (m_tribe == Tribe::Stomper) {
            if (target->remainingModels() >= 20) wounds.normal += 2;
            else if (target->remainingModels() >= 10) wounds.normal += 1;
        }
        return wounds;
    }

} // namespace SonsOfBehemat
