/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/StonehornBeastriders.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 300;

    struct TableEntry {
        int m_move;
        int m_hornsAttacks;
        int m_hoovesToWound;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 6, 2},
                    {10, 5, 3},
                    {8,  4, 3},
                    {6,  3, 4},
                    {4,  2, 4}
            };

    bool StonehornBeastriders::s_registered = false;

    Unit *StonehornBeastriders::Create(const ParameterList &parameters) {
        auto unit = new StonehornBeastriders();

        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Chaintrap);

        auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
        unit->setMawtribe(tribe);

        bool ok = unit->configure(weapon);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string StonehornBeastriders::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Chaintrap) return "Chaintrap";
            else if (parameter.intValue == Blood_Vulture) return "Blood Vulture";
        }

        return MawtribesBase::ValueToString(parameter);
    }

    int StonehornBeastriders::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Chaintrap") return Chaintrap;
        else if (enumString == "Blood Vulture") return Blood_Vulture;

        return MawtribesBase::EnumStringToInt(enumString);
    }

    void StonehornBeastriders::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Chaintrap, Blood_Vulture};
            static FactoryMethod factoryMethod = {
                    StonehornBeastriders::Create,
                    StonehornBeastriders::ValueToString,
                    StonehornBeastriders::EnumStringToInt,
                    StonehornBeastriders::ComputePoints,
                    {
                            EnumParameter("Weapon", Chaintrap, weapons),
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe)
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Stonehorn Beastriders", factoryMethod);
        }
    }

    StonehornBeastriders::StonehornBeastriders() :
            MawtribesBase("Stonehorn Beastriders", 12, g_wounds, 9, 3, false),
            m_harpoon(Weapon::Type::Missile, "Harpoon Launcher", 20, 1, 4, 3, 0, RAND_D3),
            m_chaintrap(Weapon::Type::Missile, "Chaintrap", 12, 1, 4, 3, 0, 3),
            m_vulture(Weapon::Type::Missile, "Blood Vulture", 30, 1, 0, 0, 0, 0),
            m_kicks(Weapon::Type::Melee, "Punches and Kicks", 1, 6, 4, 4, 0, 1),
            m_horns(Weapon::Type::Melee, "Rock-hard Horns", 2, 6, 4, 3, -2, 3),
            m_hooves(Weapon::Type::Melee, "Crushing Hooves", 1, RAND_D6, 3, 2, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, OGOR, STONEHORN, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, MONSTER, STONEHORN_BEASTRIDERS};
        m_weapons = {&m_harpoon, &m_chaintrap, &m_kicks, &m_horns, &m_hooves};
        m_battleFieldRole = Role::Behemoth;
        m_hasMount = true;
        m_hooves.setMount(true);
        m_horns.setMount(true);
    }

    bool StonehornBeastriders::configure(WeaponOption option) {
        auto model = new Model(g_basesize, wounds());

        m_option = option;

        model->addMissileWeapon(&m_harpoon);
        if (option == Chaintrap)
            model->addMissileWeapon(&m_chaintrap);
        else if (option == Blood_Vulture)
            model->addMissileWeapon(&m_vulture);
        model->addMeleeWeapon(&m_kicks);
        model->addMeleeWeapon(&m_horns);
        model->addMeleeWeapon(&m_hooves);

        addModel(model);

        m_points = StonehornBeastriders::ComputePoints(1);

        return true;
    }

    void StonehornBeastriders::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    int StonehornBeastriders::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void StonehornBeastriders::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_horns.setAttacks(g_damageTable[damageIndex].m_hornsAttacks);
        m_hooves.setToWound(g_damageTable[damageIndex].m_hoovesToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        MawtribesBase::onWounded();
    }

    Wounds
    StonehornBeastriders::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Earth-shattering Charge
        if (m_charged && (weapon->name() == m_horns.name() || weapon->name() == m_hooves.name())) {
            return {weapon->damage() + 1, 0};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Wounds StonehornBeastriders::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Stone Skeleton
        return ignoreWounds(wounds, 5);
    }

    void StonehornBeastriders::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        if (player == owningPlayer()) {
            if (m_meleeTarget) {
                if (m_option == Blood_Vulture) {
                    if (Dice::RollD6() >= 2) {
                        m_meleeTarget->applyDamage({0, 1}, this);
                    }
                }
            }
        }
    }

    int StonehornBeastriders::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OgorMawtribes
