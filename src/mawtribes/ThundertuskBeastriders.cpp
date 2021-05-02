/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/ThundertuskBeastriders.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 270;

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

    bool ThundertuskBeastriders::s_registered = false;

    bool ThundertuskBeastriders::AreValid(const ParameterList &parameters) {
        return true;
    }

    Unit *ThundertuskBeastriders::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
            auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Chaintrap);
            return new ThundertuskBeastriders(tribe, weapon);
        }
        return nullptr;
    }

    std::string ThundertuskBeastriders::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Chaintrap) return "Chaintrap";
            else if (parameter.intValue == Blood_Vulture) return "Blood Vulture";
        }
        return MawtribesBase::ValueToString(parameter);
    }

    int ThundertuskBeastriders::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Chaintrap") return Chaintrap;
        else if (enumString == "Blood Vulture") return Blood_Vulture;

        return MawtribesBase::EnumStringToInt(enumString);
    }

    void ThundertuskBeastriders::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Chaintrap, Blood_Vulture};
            static FactoryMethod factoryMethod = {
                    ThundertuskBeastriders::Create,
                    ThundertuskBeastriders::ValueToString,
                    ThundertuskBeastriders::EnumStringToInt,
                    ThundertuskBeastriders::ComputePoints,
                    {
                            EnumParameter("Weapon", Chaintrap, weapons),
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe)
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Thundertusk Beastriders", factoryMethod);
        }
    }

    ThundertuskBeastriders::ThundertuskBeastriders(Mawtribe tribe, WeaponOption option) :
            MawtribesBase(tribe, "Thundertusk Beastriders", 8, g_wounds, 7, 4, false, g_pointsPerUnit) {

        m_keywords = {DESTRUCTION, OGOR, THUNDERTUSK, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, MONSTER,
                      THUNDERTUSK_BEASTRIDERS};
        m_weapons = {&m_harpoon, &m_chaintrap, &m_ice, &m_kicks, &m_tusks};
        m_battleFieldRole = Role::Behemoth;
        m_hasMount = true;
        m_tusks.setMount(true);

        auto model = new Model(g_basesize, wounds());

        m_option = option;

        model->addMissileWeapon(&m_harpoon);
        if (option == Chaintrap)
            model->addMissileWeapon(&m_chaintrap);
        else if (option == Blood_Vulture)
            model->addMissileWeapon(&m_vulture);
        model->addMissileWeapon(&m_ice);
        model->addMeleeWeapon(&m_kicks);
        model->addMeleeWeapon(&m_tusks);

        addModel(model);
    }

    void ThundertuskBeastriders::onRestore() {
        MawtribesBase::onRestore();
        // Restore table-driven attributes
        onWounded();
    }

    size_t ThundertuskBeastriders::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void ThundertuskBeastriders::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_tusks.setToWound(g_damageTable[damageIndex].m_tusksToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        MawtribesBase::onWounded();
    }

    int ThundertuskBeastriders::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = MawtribesBase::targetHitModifier(weapon, attacker);
        // Numbing Chill
        if (!weapon->isMissile()) mod--;

        return mod;
    }

    void ThundertuskBeastriders::onStartShooting(PlayerId player) {
        MawtribesBase::onStartShooting(player);

        if (player == owningPlayer()) {
            if (m_meleeTarget) {
                Dice::RollResult result;
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

    int ThundertuskBeastriders::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace OgorMawtribes
