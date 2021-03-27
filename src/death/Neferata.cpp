/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/Neferata.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "LegionOfNagashPrivate.h"

namespace Death {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 11;
    static const int g_pointsPerUnit = 340;

    struct TableEntry {
        int m_move;
        int m_clawAttacks;
        double m_allureRange;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 6, 8, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {16, 6, 15},
                    {13, 5, 12},
                    {10, 4, 9},
                    {7,  3, 6},
                    {4,  2, 3}
            };

    bool NeferataMortarchOfBlood::s_registered = false;

    Unit *NeferataMortarchOfBlood::Create(const ParameterList &parameters) {
        auto unit = new NeferataMortarchOfBlood();

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        unit->setLegion(legion);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_vampireLore[0]);

        unit->configure(lore);
        return unit;
    }

    int NeferataMortarchOfBlood::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void NeferataMortarchOfBlood::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Legion", g_legions[0], g_legions),
                            EnumParameter("Lore", g_vampireLore[0], g_vampireLore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT, DEATHLORDS}
            };
            s_registered = UnitFactory::Register("Neferata, Mortarch of Blood", factoryMethod);
        }
    }

    NeferataMortarchOfBlood::NeferataMortarchOfBlood() :
            LegionOfNagashBase("Neferata, Mortarch of Blood", 16, g_wounds, 10, 4, true),
            m_akmetHar(Weapon::Type::Melee, "Akmet-har", 1, 5, 2, 3, -1, 1),
            m_akenSeth(Weapon::Type::Melee, "Aken-seth", 1, 2, 2, 3, -2, 2),
            m_skeletalClaws(Weapon::Type::Melee, "Nagadron's Skeletal Claws", 1, 6, 4, 3, -2, 2),
            m_clawsAndDaggers(Weapon::Type::Melee, "Spirits' Spectral Claws and Daggers", 1, 6, 5, 4, 0, 1) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, DEATHLORDS, MONSTER, HERO, WIZARD, MORTARCH, NEFERATA};
        m_weapons = {&m_akmetHar, &m_akenSeth, &m_skeletalClaws, &m_clawsAndDaggers};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_skeletalClaws.setMount(true);
        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    void NeferataMortarchOfBlood::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_akmetHar);
        model->addMeleeWeapon(&m_akenSeth);
        model->addMeleeWeapon(&m_skeletalClaws);
        model->addMeleeWeapon(&m_clawsAndDaggers);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    void NeferataMortarchOfBlood::onWounded() {
        Unit::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_skeletalClaws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void NeferataMortarchOfBlood::onRestore() {
        Unit::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    size_t NeferataMortarchOfBlood::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds NeferataMortarchOfBlood::weaponDamage(const Weapon *weapon, const Unit *target,
                                                 int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll >= 6) && (weapon->name() == m_clawsAndDaggers.name())) return {0, 1};
        return LegionOfNagashBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void NeferataMortarchOfBlood::onStartHero(PlayerId player) {
        LegionOfNagashBase::onStartHero(player);

        if (owningPlayer() == player) deathlyInvocations(4, 18.0);
    }

    void NeferataMortarchOfBlood::onEndCombat(PlayerId player) {

        // Mortarch of Blood
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(2);
        // TODO: if any HEROs slain heal an additional wound.

        LegionOfNagashBase::onEndCombat(player);
    }
} // namespace Death
