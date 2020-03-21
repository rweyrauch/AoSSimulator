/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/PrinceVhordrai.h>
#include <UnitFactory.h>

namespace Death {
    static const int BASESIZE = 130;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 480;

    struct TableEntry {
        int m_move;
        int m_breath;
        int m_clawAttack;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {14, 6, 7},
                    {12, RAND_D6, 6},
                    {10, 3, 5},
                    {8, RAND_D3, 4},
                    {6, 1, 3}
            };

    bool PrinceVhordrai::s_registered = false;

    Unit *PrinceVhordrai::Create(const ParameterList &parameters) {
        auto unit = new PrinceVhordrai();

        auto legion = (Legion)GetEnumParam("Legion", parameters, GrandHostOfNagash);
        unit->setLegion(legion);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int PrinceVhordrai::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void PrinceVhordrai::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Enum, "Legion", Legion::GrandHostOfNagash, Legion ::GrandHostOfNagash, Legion::LegionOfBlood, 1},
                    },
                    DEATH,
                    {SOULBLIGHT}
            };
            s_registered = UnitFactory::Register("Prince Vhordrai", factoryMethod);
        }
    }

    PrinceVhordrai::PrinceVhordrai() :
            LegionOfNagashBase("Prince Vhordrai", 14, WOUNDS, 10, 3, true),
            m_bloodlance(Weapon::Type::Melee, "Bloodlance", 2, 4, 3, 3, -2, 2),
            m_maw(Weapon::Type::Melee, "Shordemaire's Maw", 3, 3, 4, 3, -2, RAND_D6),
            m_claws(Weapon::Type::Melee, "Shordemaire's Sword-like Claws", 2, 7, 4, 3, -1, 2) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, ZOMBIE_DRAGON, MONSTER, HERO, WIZARD, PRINCE_VHORDRAI};
        m_weapons = {&m_bloodlance, &m_maw, &m_claws};
    }

    bool PrinceVhordrai::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_bloodlance);
        model->addMeleeWeapon(&m_maw);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void PrinceVhordrai::onWounded() {
        Unit::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttack);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void PrinceVhordrai::onRestore() {
        Unit::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    int PrinceVhordrai::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }
} // namespace Death
