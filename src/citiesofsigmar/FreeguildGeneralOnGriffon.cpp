/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FreeguildGeneralOnGriffon.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 105;
    static const int g_wounds = 13;
    static const int g_pointsPerUnit = 320;

    struct TableEntry {
        int m_move;
        int m_clawAttacks;
        int m_beakDamage;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {15, 6, 4},
                    {13, 5, 3},
                    {11, 4, 2},
                    {9,  3, 1},
                    {7,  2, 1}
            };

    bool FreeguildGeneralOnGriffon::s_registered = false;

    Unit *FreeguildGeneralOnGriffon::Create(const ParameterList &parameters) {
        auto unit = new FreeguildGeneralOnGriffon();

        bool shield = GetBoolParam("Freeguild Shield", parameters, true);
        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Lance);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(weapon, shield);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string FreeguildGeneralOnGriffon::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int FreeguildGeneralOnGriffon::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void FreeguildGeneralOnGriffon::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Rune_Sword, Greathammer, Lance};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Weapon", Lance, weapons),
                            BoolParameter("Freeguild Shield"),
                            EnumParameter("City", g_city[0], g_city),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Freeguild General on Griffon", factoryMethod);
        }
    }

    FreeguildGeneralOnGriffon::FreeguildGeneralOnGriffon() :
            CitizenOfSigmar("Freeguild General on Griffon", 15, g_wounds, 7, 4, true),
            m_runesword(Weapon::Type::Melee, "Sigmarite Runesword", 1, 5, 3, 4, -1, 2),
            m_greathammer(Weapon::Type::Melee, "Sigmarite Greathammer", 1, 3, 3, 3, -2, RAND_D3),
            m_lance(Weapon::Type::Melee, "Freeguild Lance", 2, 4, 3, 4, -1, 2),
            m_claws(Weapon::Type::Melee, "Razor Claws", 2, 6, 4, 3, -1, 2),
            m_beak(Weapon::Type::Melee, "Deadly Beak", 2, 2, 3, 3, -2, 4) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, MONSTER, HERO, FREEGUILD_GENERAL};
        m_weapons = {&m_runesword, &m_greathammer, &m_lance, &m_claws, &m_beak};
        m_battleFieldRole = Leader_Behemoth;
        m_hasMount = true;

        s_globalBraveryMod.connect(this, &FreeguildGeneralOnGriffon::piercingBloodroar, &m_connection);
    }

    FreeguildGeneralOnGriffon::~FreeguildGeneralOnGriffon() {
        m_connection.disconnect();
    }

    bool FreeguildGeneralOnGriffon::configure(WeaponOption weapon, bool hasShield) {
        auto model = new Model(g_basesize, wounds());
        if (weapon == Rune_Sword)
            model->addMeleeWeapon(&m_runesword);
        else if (weapon == Greathammer)
            model->addMeleeWeapon(&m_greathammer);
        else if (weapon == Lance)
            model->addMeleeWeapon(&m_lance);
        addModel(model);

        m_shield = hasShield;

        m_points = g_pointsPerUnit;

        return true;
    }

    int FreeguildGeneralOnGriffon::toSaveModifier(const Weapon *weapon) const {
        auto mod = Unit::toSaveModifier(weapon);
        if (m_shield) mod++;
        return mod;
    }

    int
    FreeguildGeneralOnGriffon::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Charging Lance
        if (m_charged && (weapon->name() == m_lance.name())) {
            return -2;
        }
        return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
    }

    int FreeguildGeneralOnGriffon::runModifier() const {
        // Skilled Rider
        auto mod = Unit::runModifier();
        if (!m_shield) mod++;
        return mod;
    }

    int FreeguildGeneralOnGriffon::chargeModifier() const {
        // Skilled Rider
        auto mod = Unit::chargeModifier();
        if (!m_shield) mod++;
        return mod;
    }

    void FreeguildGeneralOnGriffon::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    void FreeguildGeneralOnGriffon::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_beak.setDamage(g_damageTable[damageIndex].m_beakDamage);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        Unit::onWounded();
    }

    int FreeguildGeneralOnGriffon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int FreeguildGeneralOnGriffon::piercingBloodroar(const Unit *target) {
        // Piercing Bloodroar
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 8.0)) {
            return -1;
        }

        return 0;
    }

    int FreeguildGeneralOnGriffon::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }


} // namespace CitiesOfSigmar