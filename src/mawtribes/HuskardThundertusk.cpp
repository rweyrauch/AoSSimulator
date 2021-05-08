/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "mawtribes/HuskardThundertusk.h"
#include "MawtribesPrivate.h"
#include "MawtribesLore.h"

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
        if (AreValid(parameters)) {
            auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Harpoon_Launcher);
            auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
            auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_frostlordTraits[0]);
            auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_frostlordArtefacts[0]);
            auto general = GetBoolParam("General", parameters, false);
            auto mountTrait = (MountTrait) GetEnumParam("Mount Trait", parameters, g_thundertuskTraits[0]);
            auto prayer = (Prayer) GetEnumParam("Prayer", parameters, g_prayers[0]);
            return new HuskardOnThundertusk(tribe, weapon, trait, artefact, general, mountTrait, prayer);
        }
        return nullptr;
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

    HuskardOnThundertusk::HuskardOnThundertusk(Mawtribe tribe, WeaponOption option, CommandTrait trait,
                                               Artefact artefact, bool isGeneral, MountTrait mountTrait, Prayer prayer) :
            MawtribesBase(tribe, "Huskard on Thundertusk", 8, g_wounds, 8, 4, false, g_pointsPerUnit) {

        m_keywords = {DESTRUCTION, OGOR, THUNDERTUSK, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, MONSTER, HERO, PRIEST,
                      HUSKARD};
        m_hasMount = true;
        m_tusks.setMount(true);
        m_weapons = {&m_harpoon, &m_chaintrap, &m_ice, &m_kicks, &m_tusks};
        m_battleFieldRole = Role::Leader_Behemoth;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

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

        m_knownPrayers.push_back(std::unique_ptr<::Prayer>(CreatePrayer(prayer, this)));

        if (trait == CommandTrait::Master_Of_The_Mournfangs) {
            s_globalBraveryMod.connect(this, &HuskardOnThundertusk::masterOfMournfangs, &m_masterOfMournfangs);
        }
    }

    void HuskardOnThundertusk::onRestore() {
        MawtribesBase::onRestore();
        // Restore table-driven attributes
        onWounded();
    }

    size_t HuskardOnThundertusk::getDamageTableIndex() const {
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

    void HuskardOnThundertusk::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_tusks.setToWound(g_damageTable[damageIndex].m_tusksToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        MawtribesBase::onWounded();
    }

    int HuskardOnThundertusk::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = MawtribesBase::targetHitModifier(weapon, attacker);
        // Numbing Chill
        if (!weapon->isMissile()) mod--;

        return mod;
    }

    void HuskardOnThundertusk::onStartShooting(PlayerId player) {
        MawtribesBase::onStartShooting(player);

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

    int HuskardOnThundertusk::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    bool HuskardOnThundertusk::AreValid(const ParameterList &parameters) {
        return true;
    }

    void HuskardOnThundertusk::onStartHero(PlayerId player) {
        MawtribesBase::onStartHero(player);

        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        const bool isFeeding = !units.empty();

        if (isFeeding && (m_mountTrait == MountTrait::Fleshgreed)) {
            heal(1);
        }
    }

    int HuskardOnThundertusk::woundModifier() const {
        auto mod = UnitModifierInterface::woundModifier();
        if (m_mountTrait == MountTrait::Gvarnak) {
            mod++;
        }
        return mod;
    }

} // namespace OgorMawtribes
