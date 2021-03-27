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
        auto unit = new ArkhanTheBlack();

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        unit->setLegion(legion);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure();
        return unit;
    }

    std::string ArkhanTheBlack::ValueToString(const Parameter &parameter) {
        return OssiarchBonereaperBase::ValueToString(parameter);
    }

    int ArkhanTheBlack::EnumStringToInt(const std::string &enumString) {
        return OssiarchBonereaperBase::EnumStringToInt(enumString);
    }

    void ArkhanTheBlack::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ArkhanTheBlack::Create,
                    ArkhanTheBlack::ValueToString,
                    ArkhanTheBlack::EnumStringToInt,
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

    ArkhanTheBlack::ArkhanTheBlack() :
            OssiarchBonereaperBase("Arkhan the Black, Mortarch of Sacrament", 16, g_wounds, 10, 4, true),
            m_zefetKar(Weapon::Type::Melee, "Zefet-kar", 1, 1, 3, 3, -1, RAND_D3),
            m_khenashAn(Weapon::Type::Melee, "Khenash-an", 2, 1, 4, 3, -1, RAND_D3),
            m_claws(Weapon::Type::Melee, "Ebon Claws", 1, 6, 4, 3, -2, 2),
            m_clawsAndDaggers(Weapon::Type::Melee, "Spectral Claws and Dagger", 1, 6, 5, 4, 0, 1) {
        m_keywords = {DEATH, SKELETON, DEATHLORDS, MONSTER, HERO, WIZARD, MORTARCH, ARKHAN};
        m_weapons = {&m_zefetKar, &m_khenashAn, &m_claws, &m_clawsAndDaggers};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_claws.setMount(true);
        m_clawsAndDaggers.setMount(true);
        m_totalSpells = 3;
        m_totalUnbinds = 3;
    }

    void ArkhanTheBlack::configure() {
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
        const int damageIndex = getDamageTableIndex();
        m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        Unit::onWounded();
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
        Unit::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    Wounds ArkhanTheBlack::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll == 6) && (weapon->name() == m_clawsAndDaggers.name())) {
            return {0, 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int ArkhanTheBlack::castingModifier() const {
        auto mod = Unit::castingModifier();

        mod += g_damageTable[getDamageTableIndex()].m_staffCast;

        return mod;
    }

    int ArkhanTheBlack::unbindingModifier() const {
        auto mod = Unit::unbindingModifier();

        mod += g_damageTable[getDamageTableIndex()].m_staffUnbind;

        return mod;
    }

    int ArkhanTheBlack::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OssiarchBonereapers
