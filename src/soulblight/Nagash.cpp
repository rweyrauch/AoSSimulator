/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/Nagash.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 100;
    static const int g_wounds = 16;
    static const int g_pointsPerUnit = 975;

    struct TableEntry {
        int m_spells;
        int m_zefetAttacks;
        int m_castBonus;
        int m_unbindBonus;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {3, 6, 10, 13, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {5, 6, 3, 3},
                    {4, 5, 3, 2},
                    {3, 4, 2, 2},
                    {2, 3, 2, 1},
                    {1, 2, 1, 1}
            };

    bool Nagash::s_registered = false;

    Nagash::Nagash(CursedBloodline bloodline, bool isGeneral) :
            SoulblightBase(bloodline, "Nagash", 9, g_wounds, 10, 3, true, g_pointsPerUnit),
            m_gaze(Weapon::Type::Missile, "Gaze of Nagash", 12, 1, 3, 2, -1, RAND_D6),
            m_alakanash(Weapon::Type::Melee, "Alakanash", 3, 1, 3, 2, -3, RAND_D6),
            m_zefetNebtar(Weapon::Type::Melee, "Zefet-nebtar", 2, 6, 3, 3, -2, 3),
            m_clawsAndDaggers(Weapon::Type::Melee, "Spectral Claws and Daggers", 1, 6, 5, 4, 0, 1) {
        m_keywords = {DEATH, DEATHLORDS, MONSTER, HERO, PRIEST, WIZARD, NAGASH};
        m_weapons = {&m_gaze, &m_alakanash, &m_zefetNebtar, &m_clawsAndDaggers};
        m_battleFieldRole = Role::Leader_Behemoth;

        m_totalSpells = 8;
        m_totalUnbinds = 8;
        m_canRecastArcaneBoldAndMysticShield = true;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());

        model->addMissileWeapon(&m_gaze);
        model->addMeleeWeapon(&m_alakanash);
        model->addMeleeWeapon(&m_zefetNebtar);
        model->addMeleeWeapon(&m_clawsAndDaggers);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    Unit *Nagash::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Nagash(bloodline, general);
    }

    void Nagash::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines),
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS, OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Nagash", factoryMethod);
        }
    }

    size_t Nagash::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int Nagash::castingModifier() const {
        auto mod = SoulblightBase::castingModifier();

        mod += g_damageTable[getDamageTableIndex()].m_castBonus;

        return mod;
    }

    int Nagash::unbindingModifier() const {
        auto mod = SoulblightBase::unbindingModifier();

        mod += g_damageTable[getDamageTableIndex()].m_unbindBonus;

        return mod;
    }

    Wounds Nagash::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if (hitRoll == 6) {
            if (weapon->name() == m_clawsAndDaggers.name()) {
                return {0, 1};
            }
        }
        return SoulblightBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int Nagash::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void Nagash::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_zefetNebtar.setAttacks(g_damageTable[damageIndex].m_zefetAttacks);

        SoulblightBase::onWounded();
    }

    void Nagash::onRestore() {
        SoulblightBase::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

} // namespace Soulblight
