/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Cygor.h>
#include <UnitFactory.h>

namespace BeastsOfChaos {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 140;

    bool Cygor::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_boulderRange;
        int m_hornAttacts;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {8, 18, 8},
                    {6, 15, 7},
                    {5, 12, 5},
                    {4, 6,  4},
                    {3, 3,  2}
            };

    Cygor::Cygor() :
            BeastsOfChaosBase("Cygor", 8, WOUNDS, 7, 5, false),
            m_desecratedBoulder(Weapon::Type::Missile, "Desecrated Boulder", 18, 1, 4, 2, -2, RAND_D6),
            m_massiveHorns(Weapon::Type::Melee, "Massive Horns", 2, 8, 4, 3, -1, 1) {
        m_keywords = {CHAOS, BULLGOR, BEASTS_OF_CHAOS, WARHERD, MONSTER, CYGOR};
        m_weapons = {&m_desecratedBoulder, &m_massiveHorns};
    }

    bool Cygor::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_desecratedBoulder);
        model->addMeleeWeapon(&m_massiveHorns);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void Cygor::onRestore() {
        // Reset table-driven attributes
        onWounded();
    }

    Unit *Cygor::Create(const ParameterList &parameters) {
        auto unit = new Cygor();

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, BeastsOfChaosBase::None);
        unit->setGreatfray(fray);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
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
                            {ParamType::Enum, "Greatfray", BeastsOfChaosBase::None, BeastsOfChaosBase::None,
                             BeastsOfChaosBase::Gavespawn, 1},
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Cygor", factoryMethod);
        }
    }

    void Cygor::onWounded() {
        const int damageIndex = getDamageTableIndex();

        m_desecratedBoulder.setRange(g_damageTable[damageIndex].m_boulderRange);
        m_massiveHorns.setAttacks(g_damageTable[damageIndex].m_hornAttacts);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        Unit::onWounded();
    }

    int Cygor::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Rerolls Cygor::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Ghostsight
        if (target->hasKeyword(WIZARD))
            return RerollFailed;
        return Unit::toHitRerolls(weapon, target);
    }

    int Cygor::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace BeastsOfChaos
