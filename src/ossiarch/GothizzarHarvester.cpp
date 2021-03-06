/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/GothizzarHarvester.h"
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {
    static const int g_basesize = 105; // x70 oval
    static const int g_wounds = 10;
    static const int g_pointsPerUnit = 200;

    struct TableEntry {
        int m_range;
        int m_hoovesToWound;
        int m_attacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 6, 8, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {16, 2, 6},
                    {14, 3, 5},
                    {12, 4, 4},
                    {8,  5, 3},
                    {4,  6, 2}
            };

    bool GothizzarHarvester::s_registered = false;

    Unit *GothizzarHarvester::Create(const ParameterList &parameters) {
        auto option = (WeaponOption) GetEnumParam("Weapon", parameters, GothizzarHarvester::Sickles);
        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        return new GothizzarHarvester(legion, option);
    }

    std::string GothizzarHarvester::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Sickles) return "Soulcleaver Sickles";
            else if (parameter.intValue == Bludgeons) return "Soulcrusher Bludgeons";
        }
        return OssiarchBonereaperBase::ValueToString(parameter);
    }

    int GothizzarHarvester::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Soulcleaver Sickles") return Sickles;
        else if (enumString == "Soulcrusher Bludgeons") return Bludgeons;
        return OssiarchBonereaperBase::EnumStringToInt(enumString);
    }

    void GothizzarHarvester::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Sickles, Bludgeons};
            static FactoryMethod factoryMethod = {
                    GothizzarHarvester::Create,
                    GothizzarHarvester::ValueToString,
                    GothizzarHarvester::EnumStringToInt,
                    GothizzarHarvester::ComputePoints,
                    {
                            EnumParameter("Weapon", Sickles, weapons),
                            EnumParameter("Legion", g_legion[0], g_legion),
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Gothizzar Harvester", factoryMethod);
        }
    }

    GothizzarHarvester::GothizzarHarvester(Legion legion, WeaponOption option) :
            OssiarchBonereaperBase("Gothizzar Harvester", 6, g_wounds, 10, 4, false, g_pointsPerUnit) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, MONSTER, GOTHIZZAR_HARVESTER};
        m_weapons = {&m_deathsHeadMaw, &m_sickles, &m_bludgeons, &m_hoovesAndTail};
        m_battleFieldRole = Role::Behemoth;

        setLegion(legion);

        auto model = new Model(g_basesize, wounds());

        model->addMissileWeapon(&m_deathsHeadMaw);
        if (option == Sickles)
            model->addMeleeWeapon(&m_sickles);
        else if (option == Bludgeons)
            model->addMeleeWeapon(&m_bludgeons);
        model->addMeleeWeapon(&m_hoovesAndTail);

        addModel(model);
    }

    void GothizzarHarvester::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_deathsHeadMaw.setRange(g_damageTable[damageIndex].m_range);
        m_hoovesAndTail.setToWound(g_damageTable[damageIndex].m_hoovesToWound);
        m_sickles.setAttacks(g_damageTable[damageIndex].m_attacks);
        m_bludgeons.setAttacks(g_damageTable[damageIndex].m_attacks);

        OssiarchBonereaperBase::onWounded();
    }

    size_t GothizzarHarvester::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int GothizzarHarvester::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = OssiarchBonereaperBase::toHitModifier(weapon, target);

        // Soulcleaver Sickles
        if ((weapon->name() == m_sickles.name()) && (target->remainingModels() >= 5)) {
            mod++;
        }
        return mod;
    }

    Wounds
    GothizzarHarvester::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Soulcrusher Bludgeons
        if ((hitRoll == 6) && (weapon->name() == m_bludgeons.name())) {
            return {0, 2};
        }
        return OssiarchBonereaperBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int GothizzarHarvester::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void GothizzarHarvester::onRestore() {
        OssiarchBonereaperBase::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

} // namespace OssiarchBonereapers
