/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/HuskardStonehorn.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 320;

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

    bool HuskardOnStonehorn::s_registered = false;

    Unit *HuskardOnStonehorn::Create(const ParameterList &parameters) {
        auto unit = new HuskardOnStonehorn();

        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Harpoon_Launcher);

        auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
        unit->setMawtribe(tribe);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_frostlordTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_frostlordArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto mountTrait = (MountTrait) GetEnumParam("Mount Trait", parameters, g_stonehornTraits[0]);

        bool ok = unit->configure(weapon, mountTrait);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string HuskardOnStonehorn::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Harpoon_Launcher) return "Harpoon Launcher";
            else if (parameter.intValue == Chaintrap) return "Chaintrap";
            else if (parameter.intValue == Blood_Vulture) return "Blood Vulture";
        }
        return MawtribesBase::ValueToString(parameter);
    }

    int HuskardOnStonehorn::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Harpoon Launcher") return Harpoon_Launcher;
        else if (enumString == "Chaintrap") return Chaintrap;
        else if (enumString == "Blood Vulture") return Blood_Vulture;

        return MawtribesBase::EnumStringToInt(enumString);
    }

    void HuskardOnStonehorn::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Harpoon_Launcher, Chaintrap, Blood_Vulture};
            static FactoryMethod factoryMethod = {
                    HuskardOnStonehorn::Create,
                    HuskardOnStonehorn::ValueToString,
                    HuskardOnStonehorn::EnumStringToInt,
                    HuskardOnStonehorn::ComputePoints,
                    {
                            EnumParameter("Weapon", Harpoon_Launcher, weapons),
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe),
                            EnumParameter("Command Trait", g_frostlordTraits[0], g_frostlordTraits),
                            EnumParameter("Artefact", g_frostlordArtefacts[0], g_frostlordArtefacts),
                            EnumParameter("Mount Trait", g_stonehornTraits[0], g_stonehornTraits),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Huskard on Stonehorn", factoryMethod);
        }
    }

    HuskardOnStonehorn::HuskardOnStonehorn() :
            MawtribesBase("Huskard on Stonehorn", 12, g_wounds, 9, 3, false),
            m_harpoon(Weapon::Type::Missile, "Harpoon Launcher", 20, 1, 4, 3, 0, RAND_D3),
            m_chaintrap(Weapon::Type::Missile, "Chaintrap", 12, 1, 4, 3, 0, 3),
            m_vulture(Weapon::Type::Missile, "Blood Vulture", 30, 1, 0, 0, 0, 0),
            m_kicks(Weapon::Type::Melee, "Punches and Kicks", 1, 3, 3, 4, 0, 1),
            m_horns(Weapon::Type::Melee, "Rock-hard Horns", 2, 6, 4, 3, -2, 3),
            m_hooves(Weapon::Type::Melee, "Crushing Hooves", 1, RAND_D6, 3, 2, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, OGOR, STONEHORN, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, MONSTER, HERO, HUSKARD};
        m_weapons = {&m_harpoon, &m_chaintrap, &m_kicks, &m_horns, &m_hooves};
        m_battleFieldRole = Leader_Behemoth;
        m_hasMount = true;
    }

    bool HuskardOnStonehorn::configure(WeaponOption option, MountTrait mountTrait) {
        auto model = new Model(g_basesize, wounds());

        m_option = option;
        m_mountTrait = mountTrait;

        if (option == Harpoon_Launcher)
            model->addMissileWeapon(&m_harpoon);
        else if (option == Chaintrap)
            model->addMissileWeapon(&m_chaintrap);
        else if (option == Blood_Vulture)
            model->addMissileWeapon(&m_vulture);
        model->addMeleeWeapon(&m_kicks);
        model->addMeleeWeapon(&m_horns);
        model->addMeleeWeapon(&m_hooves);

        addModel(model);

        m_points = HuskardOnStonehorn::ComputePoints(1);

        return true;
    }

    void HuskardOnStonehorn::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    int HuskardOnStonehorn::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void HuskardOnStonehorn::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_horns.setAttacks(g_damageTable[damageIndex].m_hornsAttacks);
        m_hooves.setToWound(g_damageTable[damageIndex].m_hoovesToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        MawtribesBase::onWounded();
    }

    Wounds
    HuskardOnStonehorn::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Earth-shattering Charge
        if (m_charged && (weapon->name() == m_horns.name() || weapon->name() == m_hooves.name())) {
            return {weapon->damage() + 1, 0};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Wounds HuskardOnStonehorn::applyWoundSave(const Wounds &wounds, Unit* attackingUnit) {
        // Stone Skeleton
        Dice::RollResult woundSaves, mortalSaves;
        Dice::RollD6(wounds.normal, woundSaves);
        Dice::RollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(5);
        totalWounds.normal = std::max(totalWounds.normal, 0);
        totalWounds.mortal -= mortalSaves.rollsGE(5);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }

    void HuskardOnStonehorn::onStartShooting(PlayerId player) {
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

    int HuskardOnStonehorn::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OgorMawtribes
