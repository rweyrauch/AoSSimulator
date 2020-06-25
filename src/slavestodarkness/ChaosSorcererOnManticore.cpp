/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "slavestodarkness/ChaosSorcererOnManticore.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 260;

    struct TableEntry {
        int m_move;
        int m_fangsToWound;
        int m_tailAttacks;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {12, 1, 5},
                    {10, 2, 4},
                    {8,  3, 3},
                    {6,  4, 2},
                    {6,  5, 1}
            };

    bool ChaosSorcererOnManticore::s_registered = false;

    Unit *ChaosSorcererOnManticore::Create(const ParameterList &parameters) {
        auto unit = new ChaosSorcererOnManticore();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        unit->setMarkOfChaos(mark);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ChaosSorcererOnManticore::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ChaosSorcererOnManticore::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    ChaosSorcererOnManticore::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Chaos Sorcerer On Manticore", factoryMethod);
        }
    }

    ChaosSorcererOnManticore::ChaosSorcererOnManticore() :
            SlavesToDarknessBase("Chaos Sorcerer On Manticore", 12, WOUNDS, 8, 4, true),
            m_staff(Weapon::Type::Melee, "Sorcerous Reaping Staff", 2, 3, 3, 3, -1, RAND_D3),
            m_fangsAndClaws(Weapon::Type::Melee, "Honed Fangs and Claws", 1, 5, 3, 1, -1, 2),
            m_tail(Weapon::Type::Melee, "Shredding Tail", 3, 5, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, MANTICORE, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, MONSTER, HERO,
                      WIZARD, CHAOS_SORCERER_LORD};
        m_weapons = {&m_staff, &m_fangsAndClaws, &m_tail};
        m_hasMount = true;

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool ChaosSorcererOnManticore::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_fangsAndClaws);
        model->addMeleeWeapon(&m_tail);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void ChaosSorcererOnManticore::onRestore() {
        // Reset table-drive attributes
        onWounded();
    }

    void ChaosSorcererOnManticore::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_fangsAndClaws.setToWound(g_damageTable[damageIndex].m_fangsToWound);
        m_tail.setAttacks(g_damageTable[damageIndex].m_tailAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int ChaosSorcererOnManticore::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Rerolls ChaosSorcererOnManticore::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Territorial Predator
        if ((weapon->name() == m_fangsAndClaws.name()) && (target->hasKeyword(MONSTER))) {
            return RerollFailed;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    int ChaosSorcererOnManticore::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace SlavesToDarkness
