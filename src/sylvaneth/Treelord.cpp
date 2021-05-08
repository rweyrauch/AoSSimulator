/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <Dice.h>
#include <sylvaneth/Treelord.h>
#include <UnitFactory.h>
#include "SylvanethPrivate.h"

namespace Sylvaneth {
    static const int g_basesize = 105; // x70 oval
    static const int g_wounds = 10;
    static const int g_pointsPerUnit = 180;

    bool Treelord::s_registered = false;

    struct TableEntry {
        int m_stranglerootsToHit;
        int m_sweepingBlowsAttacks;
        int m_talonsToWound;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {2, 4, 2},
                    {3, 3, 2},
                    {4, 2, 3},
                    {5, 2, 3},
                    {6, 1, 4}
            };

    Treelord::Treelord(Glade glade) :
            SylvanethBase("Treelord", 6, g_wounds, 6, 3, false, g_pointsPerUnit),
            m_strangleroots(Weapon::Type::Missile, "Strangleroots", 12, 5, 2, 3, -1, 1),
            m_sweepingBlows(Weapon::Type::Melee, "Sweeping Blows", 3, 4, 3, 3, -1, RAND_D6),
            m_massiveImpalingTalons(Weapon::Type::Melee, "Massive Impaling Talons", 1, 1, 3, 2, -2, 1) {
        m_keywords = {ORDER, SYLVANETH, NOBLE_SPIRITS, MONSTER, TREELORD};
        m_weapons = {&m_strangleroots, &m_sweepingBlows, &m_massiveImpalingTalons};
        m_battleFieldRole = Role::Behemoth;

        setGlade(glade);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_strangleroots);
        model->addMeleeWeapon(&m_sweepingBlows);
        model->addMeleeWeapon(&m_massiveImpalingTalons);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    void Treelord::onWounded() {
        SylvanethBase::onWounded();
        const auto damageIndex = getDamageTableIndex();
        m_strangleroots.setToHit(g_damageTable[damageIndex].m_stranglerootsToHit);
        m_sweepingBlows.setAttacks(g_damageTable[damageIndex].m_sweepingBlowsAttacks);
        m_massiveImpalingTalons.setToWound(g_damageTable[damageIndex].m_talonsToWound);
    }

    size_t Treelord::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();

        // Our Roots Run Deep
        if (hasKeyword(OAKENBROW)) woundsInflicted += 2;

        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Unit *Treelord::Create(const ParameterList &parameters) {
        auto glade = (Glade) GetEnumParam("Glade", parameters, g_glade[0]);
        return new Treelord(glade);
    }

    void Treelord::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Treelord::Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    Treelord::ComputePoints,
                    {
                            EnumParameter("Glade", g_glade[0], g_glade),
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Treelord", factoryMethod);
        }
    }

    Wounds Treelord::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Impale
        if ((hitRoll == 6) && (weapon->name() == m_massiveImpalingTalons.name())) {
            return {0, Dice::RollD6()};
        }
        return SylvanethBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int Treelord::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void Treelord::onStartCombat(PlayerId id) {
        SylvanethBase::onStartCombat(id);

        // Groundshaking Stomp
        if (m_meleeTarget && distanceTo(m_meleeTarget) <= 3.0) {
            if (Dice::RollD6() >= 4) {
                m_meleeTarget->buffAbility(Ability::Fights_Last, 1, {GamePhase::Combat, m_battleRound, owningPlayer()});
            }
        }
    }

} // namespace Sylvaneth
