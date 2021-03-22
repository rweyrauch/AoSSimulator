/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/Nagash.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "LegionOfNagashPrivate.h"

namespace Death {
    static const int g_basesize = 100;
    static const int g_wounds = 16;
    static const int g_pointsPerUnit = 880;

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

    Nagash::Nagash() :
            LegionOfNagashBase("Nagash", 9, g_wounds, 10, 3, true),
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
    }

    Unit *Nagash::Create(const ParameterList &parameters) {
        auto unit = new Nagash();

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        unit->setLegion(legion);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Nagash::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Legion", g_legions[0], g_legions),
                            BoolParameter("General")
                    },
                    DEATH,
                    {DEATHLORDS, OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Nagash", factoryMethod);
        }
    }

    bool Nagash::configure() {
        auto model = new Model(g_basesize, wounds());

        model->addMissileWeapon(&m_gaze);
        model->addMeleeWeapon(&m_alakanash);
        model->addMeleeWeapon(&m_zefetNebtar);
        model->addMeleeWeapon(&m_clawsAndDaggers);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    int Nagash::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int Nagash::castingModifier() const {
        auto mod = LegionOfNagashBase::castingModifier();

        mod += g_damageTable[getDamageTableIndex()].m_castBonus;

        return mod;
    }

    int Nagash::unbindingModifier() const {
        auto mod = LegionOfNagashBase::unbindingModifier();

        mod += g_damageTable[getDamageTableIndex()].m_unbindBonus;

        return mod;
    }

    Wounds Nagash::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if (hitRoll == 6) {
            if (weapon->name() == m_clawsAndDaggers.name()) {
                return {0, 1};
            }
        }
        return LegionOfNagashBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int Nagash::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void Nagash::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_zefetNebtar.setAttacks(g_damageTable[damageIndex].m_zefetAttacks);

        LegionOfNagashBase::onWounded();
    }

    void Nagash::onRestore() {
        LegionOfNagashBase::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

} // namespace Death
