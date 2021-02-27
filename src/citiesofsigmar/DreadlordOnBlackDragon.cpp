/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/DreadlordOnBlackDragon.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 105;
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 300;

    struct TableEntry {
        int m_move;
        int m_jawsToWound;
        int m_clawAttacks;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {14, 1, 6},
                    {12, 2, 5},
                    {10,  3, 4},
                    {8,  4, 3},
                    {6,  5, 2}
            };

    bool DreadlordOnBlackDragon::s_registered = false;

    Unit *DreadlordOnBlackDragon::Create(const ParameterList &parameters) {
        auto unit = new DreadlordOnBlackDragon();

        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Lance_And_Shield);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(weapon);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string DreadlordOnBlackDragon::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Exile_Blade_And_Shield) {
                return "Exile Blade and Tyrant Shield";
            } else if (parameter.intValue == Exile_Blade_And_Crossbow) {
                return "Exile Blade and Repeater Crossbow";
            } else if (parameter.intValue == Lance_And_Shield) {
                return "Lance of Spite and Tyrant Shield";
            } else if (parameter.intValue == Lance_And_Crossbow) {
                return "Lance of Spite and Repeater Crossbow";
            } else if (parameter.intValue == Pair_Exile_Blades) {
                return "Pair of Exile Blades";
            }
        }
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int DreadlordOnBlackDragon::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Exile Blade and Tyrant Shield") {
            return Exile_Blade_And_Shield;
        } else if (enumString == "Exile Blade and Repeater Crossbow") {
            return Exile_Blade_And_Crossbow;
        } else if (enumString == "Lance of Spite and Tyrant Shield") {
            return Lance_And_Shield;
        } else if (enumString == "Lance of Spite and Repeater Crossbow") {
            return Lance_And_Crossbow;
        } else if (enumString == "Pair of Exile Blades") {
            return Pair_Exile_Blades;
        }
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void DreadlordOnBlackDragon::Init() {
        if (!s_registered) {
            static const std::array<int, 5> weapons = {Exile_Blade_And_Shield, Exile_Blade_And_Crossbow, Lance_And_Shield,
                                                       Lance_And_Crossbow, Pair_Exile_Blades};
            static FactoryMethod factoryMethod = {
                    DreadlordOnBlackDragon::Create,
                    DreadlordOnBlackDragon::ValueToString,
                    DreadlordOnBlackDragon::EnumStringToInt,
                    DreadlordOnBlackDragon::ComputePoints,
                    {
                            EnumParameter("Weapon", Lance_And_Shield, weapons),
                            EnumParameter("City", g_city[0], g_city),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Narcotic", g_narcotic[0], g_narcotic),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Dreadlord on Black Dragon", factoryMethod);
        }
    }

    DreadlordOnBlackDragon::DreadlordOnBlackDragon() :
            CitizenOfSigmar("Dreadlord on Black Dragon", 12, g_wounds, 8, 4, true),
            m_crossbow(Weapon::Type::Missile, "Repeater Crossbow", 16, 4, 4, 4, 0, 1),
            m_noxiousBreath(Weapon::Type::Missile, "Noxious Breath", 6, 1, 0, 0, 7, 0),
            m_blade(Weapon::Type::Melee, "Exile Blade", 1, 6, 3, 4, 0, 1),
            m_lance(Weapon::Type::Melee, "Lance of Spite", 2, 3, 3, 3, -1, 1),
            m_jaws(Weapon::Type::Melee, "Fearsome Jaws", 3, 3, 4, 2, -2, RAND_D6),
            m_claws(Weapon::Type::Melee, "Razor-sharp Claws", 2, 6, 4, 3, -1, 2) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, ORDER_SERPENTIS, MONSTER, HERO, DREADLORD, DRAGON};
        m_weapons = {&m_crossbow, &m_noxiousBreath, &m_blade, &m_lance, &m_jaws, &m_claws};
        m_battleFieldRole = Leader_Behemoth;
        m_hasMount = true;
        m_jaws.setMount(true);
        m_claws.setMount(true);
        m_noxiousBreath.setMount(true);
    }

    bool DreadlordOnBlackDragon::configure(WeaponOption weapon) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_noxiousBreath);
        model->addMeleeWeapon(&m_jaws);
        model->addMeleeWeapon(&m_claws);
        if (weapon == Lance_And_Shield) {
            model->addMeleeWeapon(&m_lance);
        } else if (weapon == Lance_And_Crossbow) {
            model->addMeleeWeapon(&m_lance);
            model->addMissileWeapon(&m_crossbow);
        } else if ((weapon == Exile_Blade_And_Shield) ||
                   (weapon == Pair_Exile_Blades)) {
            model->addMeleeWeapon(&m_blade);
        } else if (weapon == Exile_Blade_And_Crossbow) {
            model->addMeleeWeapon(&m_blade);
            model->addMissileWeapon(&m_crossbow);
        }
        addModel(model);

        m_weaponOption = weapon;
        m_points = g_pointsPerUnit;

        return true;
    }

    void DreadlordOnBlackDragon::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    void DreadlordOnBlackDragon::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_jaws.setToWound(g_damageTable[damageIndex].m_jawsToWound);
        m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        Unit::onWounded();
    }

    int DreadlordOnBlackDragon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds
    DreadlordOnBlackDragon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Lance of Spite
        if (m_charged && (weapon->name() == m_lance.name())) {
            return {2, 0};
        }

        // Noxious Breath
        if ((weapon->name() == m_noxiousBreath.name())) {
            Dice::RollResult result;
            Dice::RollD6(target->remainingModels(), result);
            return {0, result.rollsGE(6)};
        }

        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int DreadlordOnBlackDragon::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Lance of Spite
        if (m_charged && (weapon->name() == m_lance.name())) {
            return -2;
        }
        return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
    }

    int DreadlordOnBlackDragon::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace CitiesOfSigmar