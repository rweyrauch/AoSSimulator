/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/KairosFateweaver.h"
#include "TzeentchPrivate.h"

namespace Tzeentch {
    static const int g_basesize = 100;
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 400;

    struct TableEntry {
        int m_move;
        int m_staffToWound;
        int m_giftOfChange;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 1, 6},
                    {10, 2, RAND_D6},
                    {9,  3, 3},
                    {8,  4, RAND_D3},
                    {7,  5, 1}
            };

    bool KairosFateweaver::s_registered = false;

    Unit *KairosFateweaver::Create(const ParameterList &parameters) {
        auto unit = new KairosFateweaver();

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, g_changeCoven[0]);
        unit->setChangeCoven(coven);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void KairosFateweaver::Init() {
        if (!s_registered) {
            static const FactoryMethod factoryMethod = {
                    KairosFateweaver::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    KairosFateweaver::ComputePoints,
                    {
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                            EnumParameter("Lore", g_loreOfChange[0], g_loreOfChange),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Kairos Fateweaver", factoryMethod);
        }
    }

    KairosFateweaver::KairosFateweaver() :
            TzeentchBase("Kairos Fateweaver", 12, g_wounds, 10, 4, true),
            m_staff(Weapon::Type::Melee, "Staff of Tomorrow", 3, 3, 3, 1, -1, 2),
            m_beakAndTalons(Weapon::Type::Melee, "Beak and Claws", 1, 5, 4, 3, -1, 2) {
        m_keywords = {CHAOS, DAEMON, TZEENTCH, MONSTER, HERO, WIZARD, LORD_OF_CHANGE, KAIROS_FATEWEAVER};
        m_weapons = {&m_staff, &m_beakAndTalons};
        m_battleFieldRole = Leader_Behemoth;

        m_totalSpells = 3;
        m_totalUnbinds = 3;
    }

    bool KairosFateweaver::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_beakAndTalons);
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    void KairosFateweaver::onRestore() {
        // Reset table-drive attributes
        onWounded();
    }

    void KairosFateweaver::onWounded() {
        TzeentchBase::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_staff.setToWound(g_damageTable[damageIndex].m_staffToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int KairosFateweaver::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int KairosFateweaver::rollCasting() const {
        // Mastery of Magic
        auto r0 = Dice::RollD6();
        auto r1 = Dice::RollD6();
        return std::max(r0, r1) * 2 + castingModifier();
    }

    int KairosFateweaver::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // Tzeentch