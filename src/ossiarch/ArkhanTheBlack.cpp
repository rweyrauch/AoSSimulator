/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "ossiarch/ArkhanTheBlack.h"
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {
    static const int g_basesize = 105;
    static const int g_wounds = 11;
    static const int g_pointsPerUnit = 360;

    struct TableEntry {
        int m_move;
        int m_clawAttacks;
        int m_staffCast;
        int m_staffUnbind;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 6, 8, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {16, 6, 2, 2},
                    {13, 5, 2, 1},
                    {10, 4, 1, 1},
                    {7,  3, 1, 0},
                    {4,  2, 0, 0}
            };

    bool ArkhanTheBlack::s_registered = false;

    Unit *ArkhanTheBlack::Create(const ParameterList &parameters) {
        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new ArkhanTheBlack(legion, general);
    }

    void ArkhanTheBlack::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ArkhanTheBlack::Create,
                    OssiarchBonereaperBase::ValueToString,
                    OssiarchBonereaperBase::EnumStringToInt,
                    ArkhanTheBlack::ComputePoints,
                    {
                            EnumParameter("Legion", g_legion[0], g_legion),
                            BoolParameter("General")
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS, DEATHLORDS}
            };
            s_registered = UnitFactory::Register("Arkhan the Black, Mortarch of Sacrament", factoryMethod);
        }
    }

    ArkhanTheBlack::ArkhanTheBlack(Legion legion, bool isGeneral) :
            OssiarchBonereaperBase("Arkhan the Black, Mortarch of Sacrament", 16, g_wounds, 10, 4, true) {
        m_keywords = {DEATH, SKELETON, DEATHLORDS, MONSTER, HERO, WIZARD, MORTARCH, ARKHAN};
        m_weapons = {&m_zefetKar, &m_khenashAn, &m_claws, &m_clawsAndDaggers};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_claws.setMount(true);
        m_clawsAndDaggers.setMount(true);
        m_totalSpells = 3;
        m_totalUnbinds = 3;

        setLegion(legion);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_zefetKar);
        model->addMeleeWeapon(&m_khenashAn);
        model->addMeleeWeapon(&m_claws);
        model->addMeleeWeapon(&m_clawsAndDaggers);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    void ArkhanTheBlack::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        OssiarchBonereaperBase::onWounded();
    }

    size_t ArkhanTheBlack::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void ArkhanTheBlack::onRestore() {
        OssiarchBonereaperBase::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    Wounds ArkhanTheBlack::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll == 6) && (weapon->name() == m_clawsAndDaggers.name())) {
            return {0, 1};
        }
        return OssiarchBonereaperBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int ArkhanTheBlack::castingModifier() const {
        auto mod = OssiarchBonereaperBase::castingModifier();

        mod += g_damageTable[getDamageTableIndex()].m_staffCast;

        return mod;
    }

    int ArkhanTheBlack::unbindingModifier() const {
        auto mod = OssiarchBonereaperBase::unbindingModifier();

        mod += g_damageTable[getDamageTableIndex()].m_staffUnbind;

        return mod;
    }

    int ArkhanTheBlack::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OssiarchBonereapers
