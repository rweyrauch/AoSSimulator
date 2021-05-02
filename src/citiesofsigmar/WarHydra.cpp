/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/WarHydra.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 105;
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 170;

    struct TableEntry {
        int m_move;
        int m_breathAttacks;
        int m_fangAttacks;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 5, 7, 9, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {8, 6, 6},
                    {7, 5, 5},
                    {6, 4, 4},
                    {5, 3, 3},
                    {4, 2, 2}
            };

    bool WarHydra::s_registered = false;

    Unit *WarHydra::Create(const ParameterList &parameters) {
        auto unit = new WarHydra();

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        unit->configure();
        return unit;
    }

    std::string WarHydra::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int WarHydra::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void WarHydra::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    WarHydra::Create,
                    WarHydra::ValueToString,
                    WarHydra::EnumStringToInt,
                    WarHydra::ComputePoints,
                    {
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("War Hydra", factoryMethod);
        }
    }

    WarHydra::WarHydra() :
            CitizenOfSigmar("War Hydra", 8, g_wounds, 6, 4, false, g_pointsPerUnit),
            m_fieryBreath(Weapon::Type::Missile, "Fiery Breath", 9, 6, 3, 3, -1, 1),
            m_fangs(Weapon::Type::Melee, "Razor-sharp Fangs", 2, 6, 4, 3, -1, RAND_D3),
            m_limbs(Weapon::Type::Melee, "Clawed Limbs", 1, 2, 3, 3, -1, 1),
            m_goadAndWhips(Weapon::Type::Melee, "Cruel Goads and Whips", 2, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, ORDER_SERPENTIS, MONSTER, WAR_HYDRA};
        m_weapons = {&m_fieryBreath, &m_fangs, &m_limbs, &m_goadAndWhips};
        m_battleFieldRole = Role::Behemoth;
    }

    bool WarHydra::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_fieryBreath);
        model->addMeleeWeapon(&m_fangs);
        model->addMeleeWeapon(&m_limbs);
        model->addMeleeWeapon(&m_goadAndWhips);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    void WarHydra::onRestore() {
        CitizenOfSigmar::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    void WarHydra::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_fieryBreath.setAttacks(g_damageTable[damageIndex].m_breathAttacks);
        m_fangs.setAttacks(g_damageTable[damageIndex].m_fangAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        CitizenOfSigmar::onWounded();
    }

    size_t WarHydra::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int WarHydra::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void WarHydra::onStartHero(PlayerId player) {
        CitizenOfSigmar::onStartHero(player);

        // Sever One Head, Another Takes Its Place
        if (owningPlayer() == player) {
            heal(Dice::RollD3());
        }
    }

    int WarHydra::rollChargeDistance() {
        if (useQuickWithTheLash()) {
            auto rolls = Dice::RollD6(3);
            std::sort(rolls.begin(), rolls.end());

            m_unmodifiedChargeRoll = rolls[1] + rolls[2];
            if (m_movementRules.contains(MovementRule::Halve_Charge_Roll)) {
                if (!m_movementRules.at(MovementRule::Halve_Charge_Roll).empty()) {
                    if (m_movementRules.at(MovementRule::Halve_Charge_Roll).front().allowed) {
                        m_unmodifiedChargeRoll = (m_unmodifiedChargeRoll + 1) / 2; // Round up
                    }
                }
            }
            if ((rolls[0] == rolls[1]) && (rolls[0] == rolls[2])) {
                // take a mortal wound
                applyDamage({0, 1, Wounds::Source::Ability}, this);
            }
            return m_unmodifiedChargeRoll + chargeModifier();
        }
        return CitizenOfSigmar::rollChargeDistance();
    }

    bool WarHydra::useQuickWithTheLash() const {
        return (remainingWounds() > 3);
    }

} // namespace CitiesOfSigmar