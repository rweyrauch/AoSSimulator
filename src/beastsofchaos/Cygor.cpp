/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Cygor.h>
#include <UnitFactory.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 140;

    bool Cygor::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_boulderRange;
        int m_hornAttacts;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {8, 18, 8},
                    {6, 15, 7},
                    {5, 12, 5},
                    {4, 6,  4},
                    {3, 3,  2}
            };

    Cygor::Cygor() :
            BeastsOfChaosBase("Cygor", 8, g_wounds, 7, 5, false),
            m_desecratedBoulder(Weapon::Type::Missile, "Desecrated Boulder", 18, 1, 4, 2, -2, RAND_D6),
            m_massiveHorns(Weapon::Type::Melee, "Massive Horns", 2, 8, 4, 3, -1, 1) {
        m_keywords = {CHAOS, BULLGOR, BEASTS_OF_CHAOS, WARHERD, MONSTER, CYGOR};
        m_weapons = {&m_desecratedBoulder, &m_massiveHorns};
        m_battleFieldRole = Role::Behemoth;
    }

    bool Cygor::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_desecratedBoulder);
        model->addMeleeWeapon(&m_massiveHorns);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    void Cygor::onRestore() {
        // Reset table-driven attributes
        onWounded();
    }

    Unit *Cygor::Create(const ParameterList &parameters) {
        auto unit = new Cygor();

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        unit->setGreatfray(fray);

        unit->configure();
        return unit;
    }

    void Cygor::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Cygor", factoryMethod);
        }
    }

    void Cygor::onWounded() {
        const auto damageIndex = getDamageTableIndex();

        m_desecratedBoulder.setRange(g_damageTable[damageIndex].m_boulderRange);
        m_massiveHorns.setAttacks(g_damageTable[damageIndex].m_hornAttacts);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        BeastsOfChaosBase::onWounded();
    }

    size_t Cygor::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Rerolls Cygor::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Ghostsight
        if (target->hasKeyword(WIZARD))
            return Rerolls::Failed;
        return BeastsOfChaosBase::toHitRerolls(weapon, target);
    }

    int Cygor::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void Cygor::onUnboundSpell(Unit *caster, int castRoll) {
        BeastsOfChaosBase::onUnboundSpell(caster, castRoll);

        // Soul-eater
        caster->applyDamage({0, 1}, this);
    }

} // namespace BeastsOfChaos
