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

    bool HuskardOnStonehorn::AreValid(const ParameterList &parameters) {
        return true;
    }

    Unit *HuskardOnStonehorn::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Harpoon_Launcher);
            auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
            auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_frostlordTraits[0]);
            auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_frostlordArtefacts[0]);
            auto general = GetBoolParam("General", parameters, false);
            auto mountTrait = (MountTrait) GetEnumParam("Mount Trait", parameters, g_stonehornTraits[0]);
            return new HuskardOnStonehorn(tribe, weapon, trait, artefact, general, mountTrait);
        }
        return nullptr;
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

    HuskardOnStonehorn::HuskardOnStonehorn(Mawtribe tribe, WeaponOption option, CommandTrait trait, Artefact artefact,
                                           bool isGeneral,
                                           MountTrait mountTrait) :
            MawtribesBase(tribe, "Huskard on Stonehorn", 12, g_wounds, 9, 3, false) {
        m_keywords = {DESTRUCTION, OGOR, STONEHORN, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, MONSTER, HERO, HUSKARD};
        m_weapons = {&m_harpoon, &m_chaintrap, &m_kicks, &m_horns, &m_hooves};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_hooves.setMount(true);
        m_horns.setMount(true);

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

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

        if (trait == CommandTrait::Master_Of_The_Mournfangs) {
            s_globalBraveryMod.connect(this, &HuskardOnStonehorn::masterOfMournfangs, &m_masterOfMournfangs);
        }

        m_points = HuskardOnStonehorn::ComputePoints(1);
    }

    void HuskardOnStonehorn::onRestore() {
        MawtribesBase::onRestore();
        // Restore table-driven attributes
        onWounded();
    }

    size_t HuskardOnStonehorn::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        if (m_commandTrait == CommandTrait::Skilled_Rider) {
            woundsInflicted /= 2;
        }
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void HuskardOnStonehorn::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_horns.setAttacks(g_damageTable[damageIndex].m_hornsAttacks);
        m_hooves.setToWound(g_damageTable[damageIndex].m_hoovesToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        MawtribesBase::onWounded();
    }

    Wounds
    HuskardOnStonehorn::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Earth-shattering Charge
        if (m_charged && (weapon->name() == m_horns.name() || weapon->name() == m_hooves.name())) {
            return {weapon->damage() + 1, 0};
        }
        return MawtribesBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    Wounds HuskardOnStonehorn::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Stone Skeleton
        return ignoreWounds(wounds, 5);
    }

    void HuskardOnStonehorn::onStartShooting(PlayerId player) {
        MawtribesBase::onStartShooting(player);

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

    int HuskardOnStonehorn::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = MawtribesBase::toHitModifier(weapon, target);
        if ((weapon->name() == m_horns.name()) && (m_mountTrait == MountTrait::Black_Clatterhorn)) {
            mod++;
        }
        return mod;
    }

    int HuskardOnStonehorn::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto rend = MawtribesBase::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
        if ((weapon->name() == m_hooves.name()) && (m_mountTrait == MountTrait::Frosthoof_Bull)) {
            rend--;
        }
        return rend;
    }

    int HuskardOnStonehorn::woundModifier() const {
        auto mod = MawtribesBase::woundModifier();
        if (m_mountTrait == MountTrait::Rockmane_Elder) {
            mod++;
        }
        return mod;
    }

} // namespace OgorMawtribes
