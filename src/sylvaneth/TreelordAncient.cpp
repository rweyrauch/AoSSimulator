/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/TreelordAncient.h>
#include <UnitFactory.h>
#include <iostream>
#include <spells/MysticShield.h>

namespace Sylvaneth {
    static const int BASESIZE = 105; // x70 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 260;

    bool TreelordAncient::s_registered = false;

    struct TableEntry {
        int m_staffToHit;
        int m_sweepingBlowsAttacks;
        int m_talonToWound;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 6, 8, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {2, 3, 2},
                    {3, 2, 2},
                    {4, 2, 3},
                    {5, 1, 3},
                    {6, 1, 4}
            };

    TreelordAncient::TreelordAncient() :
            SylvanethBase("Treelord Ancient", 5, WOUNDS, 9, 3, false),
            m_doomTendrilStaff(Weapon::Type::Missile, "Doom Tendril Staff", 18, 1, 2, 3, -1, RAND_D6),
            m_sweepingBlows(Weapon::Type::Melee, "Sweeping Blows", 3, 3, 3, 3, -1, RAND_D6),
            m_massiveImpalingTalons(Weapon::Type::Melee, "Massive Impaling Talons", 1, 1, 3, 2, -2, 1) {
        m_keywords = {ORDER, SYLVANETH, NOBLE_SPIRITS, MONSTER, HERO, WIZARD, TREELORD_ANCIENT};
        m_weapons = {&m_doomTendrilStaff, &m_sweepingBlows, &m_massiveImpalingTalons};

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool TreelordAncient::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_doomTendrilStaff);
        model->addMeleeWeapon(&m_sweepingBlows);
        model->addMeleeWeapon(&m_massiveImpalingTalons);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void TreelordAncient::onStartHero(PlayerId id) {
    }

    void TreelordAncient::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_doomTendrilStaff.setToHit(g_damageTable[damageIndex].m_staffToHit);
        m_sweepingBlows.setAttacks(g_damageTable[damageIndex].m_sweepingBlowsAttacks);
        m_massiveImpalingTalons.setToWound(g_damageTable[damageIndex].m_talonToWound);
    }

    int TreelordAncient::getDamageTableIndex() const {
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

    Unit *TreelordAncient::Create(const ParameterList &parameters) {
        auto unit = new TreelordAncient();

        auto glade = (Glade) GetEnumParam("Glade", parameters, SylvanethBase::None);
        unit->setGlade(glade);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void TreelordAncient::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TreelordAncient::Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    TreelordAncient::ComputePoints,
                    {
                            {ParamType::Enum, "Glade", SylvanethBase::None, SylvanethBase::None,
                             SylvanethBase::Harvestboon, 1},
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Treelord Ancient", factoryMethod);
        }
    }

    Wounds TreelordAncient::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Impale
        if ((hitRoll == 6) && (weapon->name() == m_massiveImpalingTalons.name())) {
            return {0, Dice::rollD6()};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int TreelordAncient::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace Sylvaneth
