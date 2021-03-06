/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <greenskinz/OrrukWarbossWyvern.h>
#include <UnitFactory.h>

namespace Greenskinz {
    static const int g_basesize = 100;
    static const int g_wounds = 11;
    static const int g_pointsPerUnit = 240;

    struct TableEntry {
        int m_move;
        int m_clawsAttacks;
        int m_tailToWound;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 6, 8, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 5, 2},
                    {10, 4, 3},
                    {8,  3, 4},
                    {6,  2, 5},
                    {4,  1, 6}
            };

    bool OrrukWarbossOnWyvern::s_registered = false;

    OrrukWarbossOnWyvern::OrrukWarbossOnWyvern(bool pairedChoppas, bool isGeneral) :
            Unit("Orruk Warboss on Wyvern", 12, g_wounds, 7, 4, true, g_pointsPerUnit),
            m_bossChoppa(Weapon::Type::Melee, "Boss Choppa", 1, 6, 3, 3, -1, 1),
            m_hornsClawsAndTeeth(Weapon::Type::Melee, "Wyvern's Horns, Claws and Teeth", 2, 5, 4, 3, -1, 2),
            m_barbedTail(Weapon::Type::Melee, "Wyvern's Barbed, Venomous Tail", 3, 2, 4, 2, -1, 3) {
        m_keywords = {DESTRUCTION, ORRUK, WYVERN, GREENSKINZ, MONSTER, HERO, ORRUK_WARBOSS};
        m_weapons = {&m_bossChoppa, &m_hornsClawsAndTeeth, &m_barbedTail};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_hornsClawsAndTeeth.setMount(true);
        m_barbedTail.setMount(true);

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());

        m_pairedChoppas = pairedChoppas;

        model->addMeleeWeapon(&m_bossChoppa);
        model->addMeleeWeapon(&m_hornsClawsAndTeeth);
        model->addMeleeWeapon(&m_barbedTail);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *OrrukWarbossOnWyvern::Create(const ParameterList &parameters) {
        bool pairedChoppas = GetBoolParam("Two Boss Choppas", parameters, false);
        auto general = GetBoolParam("General", parameters, false);
        return new OrrukWarbossOnWyvern(pairedChoppas, general);
    }

    void OrrukWarbossOnWyvern::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    OrrukWarbossOnWyvern::Create,
                    nullptr,
                    nullptr,
                    OrrukWarbossOnWyvern::ComputePoints,
                    {
                            BoolParameter("Two Boss Choppas"),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {GREENSKINZ}
            };
            s_registered = UnitFactory::Register("Orruk Warboss on Wyvern", factoryMethod);
        }
    }

    size_t OrrukWarbossOnWyvern::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void OrrukWarbossOnWyvern::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_hornsClawsAndTeeth.setAttacks(g_damageTable[damageIndex].m_clawsAttacks);
        m_barbedTail.setToWound(g_damageTable[damageIndex].m_tailToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        Unit::onWounded();
    }

    void OrrukWarbossOnWyvern::onRestore() {
        // Reset table-drive attributes
        onWounded();
    }

    Rerolls OrrukWarbossOnWyvern::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        // Boss Shield
        if (!m_pairedChoppas) {
            return Rerolls::Failed;
        }
        return Unit::toSaveRerolls(weapon, attacker);
    }

    int
    OrrukWarbossOnWyvern::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = Unit::extraAttacks(attackingModel, weapon, target);
        // Choppa Boss
        if (m_pairedChoppas) {
            attacks += 2;
        }
        return attacks;
    }

    int OrrukWarbossOnWyvern::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Greenskinz
