/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/Treelord.h>
#include <UnitFactory.h>
#include "SylvanethPrivate.h"

namespace Sylvaneth {
    static const int BASESIZE = 105; // x70 oval
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 180;

    bool Treelord::s_registered = false;

    struct TableEntry {
        int m_stranglerootsToHit;
        int m_sweepingBlowsAttacks;
        int m_talonsToWound;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {2, 4, 2},
                    {3, 3, 2},
                    {4, 2, 3},
                    {5, 2, 3},
                    {6, 1, 4}
            };

    Treelord::Treelord() :
            SylvanethBase("Treelord", 6, WOUNDS, 6, 3, false),
            m_strangleroots(Weapon::Type::Missile, "Strangleroots", 12, 5, 2, 3, -1, 1),
            m_sweepingBlows(Weapon::Type::Melee, "Sweeping Blows", 3, 4, 3, 3, -1, RAND_D6),
            m_massiveImpalingTalons(Weapon::Type::Melee, "Massive Impaling Talons", 1, 1, 3, 2, -2, 1) {
        m_keywords = {ORDER, SYLVANETH, NOBLE_SPIRITS, MONSTER, TREELORD};
        m_weapons = {&m_strangleroots, &m_sweepingBlows, &m_massiveImpalingTalons};
        m_battleFieldRole = Behemoth;
    }

    bool Treelord::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_strangleroots);
        model->addMeleeWeapon(&m_sweepingBlows);
        model->addMeleeWeapon(&m_massiveImpalingTalons);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void Treelord::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_strangleroots.setToHit(g_damageTable[damageIndex].m_stranglerootsToHit);
        m_sweepingBlows.setAttacks(g_damageTable[damageIndex].m_sweepingBlowsAttacks);
        m_massiveImpalingTalons.setToWound(g_damageTable[damageIndex].m_talonsToWound);
    }

    int Treelord::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();

        // Our Roots Run Deep
        if (hasKeyword(OAKENBROW)) woundsInflicted += 2;

        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Unit *Treelord::Create(const ParameterList &parameters) {
        auto unit = new Treelord();

        auto glade = (Glade) GetEnumParam("Glade", parameters, g_glade[0]);
        unit->setGlade(glade);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

    Wounds Treelord::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Impale
        if ((hitRoll == 6) && (weapon->name() == m_massiveImpalingTalons.name())) {
            return {0, Dice::rollD6()};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int Treelord::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Sylvaneth
