/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Kharibdyss.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 105;
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 170;

    struct TableEntry {
        int m_move;
        int m_tentacleAttacks;
        int m_tailToWound;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {1, 3, 5, 7, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {7, 6, 2},
                    {6, 5, 3},
                    {5, 4, 4},
                    {5, 3, 5},
                    {4, 2, 6}
            };

    bool Kharibdyss::s_registered = false;

    Unit *Kharibdyss::Create(const ParameterList &parameters) {
        auto unit = new Kharibdyss();

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string Kharibdyss::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Kharibdyss::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void Kharibdyss::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Kharibdyss", factoryMethod);
        }
    }

    Kharibdyss::Kharibdyss() :
            CitizenOfSigmar("Kharibdyss", 7, g_wounds, 6, 4, false),
            m_tentacles(Weapon::Type::Melee, "Fanged Tentacles", 3, 6, 4, 3, -1, 2),
            m_tail(Weapon::Type::Melee, "Spiked Tail", 2, RAND_D6, 4, 2, 0, 1),
            m_limbs(Weapon::Type::Melee, "Clawed Limbs", 1, 2, 3, 3, -1, 1),
            m_goadsAndWhips(Weapon::Type::Melee, "Cruel Goads and Whips", 2, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, SCOURGE_PRIVATEERS, MONSTER, KHARIBDYSS};
        m_weapons = {&m_tentacles, &m_tail, &m_limbs, &m_goadsAndWhips};
        m_battleFieldRole = Behemoth;

        s_globalBraveryMod.connect(this, &Kharibdyss::abyssalHowl, &m_connection);
    }

    Kharibdyss::~Kharibdyss() {
        m_connection.disconnect();
    }

    bool Kharibdyss::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_tentacles);
        model->addMeleeWeapon(&m_tail);
        model->addMeleeWeapon(&m_limbs);
        model->addMeleeWeapon(&m_goadsAndWhips);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    void Kharibdyss::onRestore() {
        CitizenOfSigmar::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    void Kharibdyss::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_tentacles.setAttacks(g_damageTable[damageIndex].m_tentacleAttacks);
        m_tail.setToWound(g_damageTable[damageIndex].m_tailToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        CitizenOfSigmar::onWounded();
    }

    int Kharibdyss::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int Kharibdyss::abyssalHowl(const Unit *target) {
        // Abyssal Howl
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 12.0)) {
            return -1;
        }

        return 0;
    }

    int Kharibdyss::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void Kharibdyss::onEndCombat(PlayerId player) {
        CitizenOfSigmar::onEndCombat(player);

        // Feast of Bones
        if (m_currentRecord.m_enemyModelsSlain > 0) {
            heal(Dice::RollD3());
        }
    }

    int Kharibdyss::rollChargeDistance()  {
        if (useQuickWithTheLash()) {
            auto rolls = Dice::RollD6(3);
            std::sort(rolls.begin(), rolls.end());

            m_unmodifiedChargeRoll = rolls[1] + rolls[2];
            if (!m_movementRules[Halve_Charge_Roll].empty()) {
                if (m_movementRules[Halve_Charge_Roll].front().allowed) {
                    m_unmodifiedChargeRoll = (m_unmodifiedChargeRoll + 1) / 2; // Round up
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

    bool Kharibdyss::useQuickWithTheLash() const {
        return (remainingWounds() > 3);
    }

} // namespace CitiesOfSigmar