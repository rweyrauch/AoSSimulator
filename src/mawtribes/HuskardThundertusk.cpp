/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/HuskardThundertusk.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 300;

    struct TableEntry {
        int m_move;
        int m_ice;
        int m_tusksToWound;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {8, 12, 2},
                    {7, 10, 3},
                    {6, 8,  3},
                    {5, 6,  4},
                    {4, 4,  4}
            };

    bool HuskardOnThundertusk::s_registered = false;

    Unit *HuskardOnThundertusk::Create(const ParameterList &parameters) {
        auto unit = new HuskardOnThundertusk();

        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Harpoon_Launcher);

        auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
        unit->setMawtribe(tribe);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_frostlordTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_frostlordArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto mountTrait = (MountTrait) GetEnumParam("Mount Trait", parameters, g_thundertuskTraits[0]);
        auto prayer = (Prayer) GetEnumParam("Prayer", parameters, g_prayers[0]);

        bool ok = unit->configure(weapon, mountTrait, prayer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string HuskardOnThundertusk::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Harpoon_Launcher) return "Harpoon Launcher";
            else if (parameter.intValue == Chaintrap) return "Chaintrap";
            else if (parameter.intValue == Blood_Vulture) return "Blood Vulture";
        }
        return MawtribesBase::ValueToString(parameter);
    }

    int HuskardOnThundertusk::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Harpoon Launcher") return Harpoon_Launcher;
        else if (enumString == "Chaintrap") return Chaintrap;
        else if (enumString == "Blood Vulture") return Blood_Vulture;

        return MawtribesBase::EnumStringToInt(enumString);
    }

    void HuskardOnThundertusk::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Harpoon_Launcher, Chaintrap, Blood_Vulture};
            static FactoryMethod factoryMethod = {
                    HuskardOnThundertusk::Create,
                    HuskardOnThundertusk::ValueToString,
                    HuskardOnThundertusk::EnumStringToInt,
                    HuskardOnThundertusk::ComputePoints,
                    {
                            EnumParameter("Weapon", Harpoon_Launcher, weapons),
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe),
                            EnumParameter("Command Trait", g_frostlordTraits[0], g_frostlordTraits),
                            EnumParameter("Artefact", g_frostlordArtefacts[0], g_frostlordArtefacts),
                            EnumParameter("Mount Trait", g_thundertuskTraits[0], g_thundertuskTraits),
                            EnumParameter("Prayer", g_prayers[0], g_prayers),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Huskard on Thundertusk", factoryMethod);
        }
    }

    HuskardOnThundertusk::HuskardOnThundertusk() :
            MawtribesBase("Huskard on Thundertusk", 8, g_wounds, 8, 4, false),
            m_harpoon(Weapon::Type::Missile, "Harpoon Launcher", 20, 1, 4, 3, 0, RAND_D3),
            m_chaintrap(Weapon::Type::Missile, "Chaintrap", 12, 1, 4, 3, 0, 3),
            m_vulture(Weapon::Type::Missile, "Blood Vulture", 30, 1, 0, 0, 0, 0),
            m_ice(Weapon::Type::Missile, "Frost-wreathed Ice", 18, 0, 0, 0, 0, 0),
            m_kicks(Weapon::Type::Melee, "Punches and Kicks", 1, 3, 3, 4, 0, 1),
            m_tusks(Weapon::Type::Melee, "Colossal Tusks", 2, 4, 3, 2, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, OGOR, THUNDERTUSK, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, MONSTER, HERO, PRIEST,
                      HUSKARD};
        m_hasMount = true;
        m_weapons = {&m_harpoon, &m_chaintrap, &m_ice, &m_kicks, &m_tusks};
        m_battleFieldRole = Leader_Behemoth;
    }

    bool HuskardOnThundertusk::configure(WeaponOption option, MountTrait mountTrait, Prayer prayer) {
        auto model = new Model(g_basesize, wounds());

        m_option = option;
        m_prayer = prayer;
        m_mountTrait = mountTrait;

        if (option == Harpoon_Launcher)
            model->addMissileWeapon(&m_harpoon);
        else if (option == Chaintrap)
            model->addMissileWeapon(&m_chaintrap);
        else if (option == Blood_Vulture)
            model->addMissileWeapon(&m_vulture);
        model->addMissileWeapon(&m_ice);
        model->addMeleeWeapon(&m_kicks);
        model->addMeleeWeapon(&m_tusks);

        addModel(model);

        m_points = HuskardOnThundertusk::ComputePoints(1);

        return true;
    }

    void HuskardOnThundertusk::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    int HuskardOnThundertusk::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void HuskardOnThundertusk::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_tusks.setToWound(g_damageTable[damageIndex].m_tusksToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        MawtribesBase::onWounded();
    }

    int HuskardOnThundertusk::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);
        // Numbing Chill
        if (!weapon->isMissile()) mod--;

        return mod;
    }

    void HuskardOnThundertusk::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        if (player == owningPlayer()) {
            if (m_meleeTarget) {
                Dice::RollResult result;

                // Blasts of Frost-wreathed Ice
                Dice::RollD6(g_damageTable[getDamageTableIndex()].m_ice, result);
                int toWound = 6;
                if (m_meleeTarget->remainingModels() >= 20) toWound -= 2;
                else if (m_meleeTarget->remainingModels() >= 10) toWound -= 1;

                Wounds wounds = {0, result.rollsGE(toWound)};
                m_meleeTarget->applyDamage(wounds, this);

                if (m_option == Blood_Vulture) {
                    if (Dice::RollD6() >= 2) {
                        m_meleeTarget->applyDamage({0, 1}, this);
                    }
                }
            }
        }
    }

    int HuskardOnThundertusk::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OgorMawtribes
