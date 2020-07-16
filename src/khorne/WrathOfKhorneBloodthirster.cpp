/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/WrathOfKhorneBloodthirster.h>
#include <UnitFactory.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 300;

    struct TableEntry {
        int m_move;
        int m_flailDamage;
        int m_axeToWound;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {10, 6,       2},
                    {9,  3,       3},
                    {8,  3,       3},
                    {7,  RAND_D3, 4},
                    {6,  RAND_D3, 4}
            };

    bool WrathOfKhorneBloodthirster::s_registered = false;

    WrathOfKhorneBloodthirster::WrathOfKhorneBloodthirster() :
            KhorneBase("Wrath of Khorne Bloodthirster", 10, WOUNDS, 10, 4, true),
            m_bloodflail(Weapon::Type::Missile, "Bloodflail", 12, 1, 3, 3, -1, 6),
            m_mightyAxeOfKhorne(Weapon::Type::Melee, "Mighty Axe of Khorne", 2, 6, 3, 2, -2, RAND_D3),
            m_breath(Weapon::Type::Missile, "Hellfire Breath", 8, 1, 0, 0, 0, 0) {
        m_keywords = {CHAOS, DAEMON, BLOODTHIRSTER, KHORNE, MONSTER, HERO, WRATH_OF_KHORNE_BLOODTHIRSTER};
        m_weapons = {&m_bloodflail, &m_mightyAxeOfKhorne, &m_breath};
        m_battleFieldRole = LeaderBehemoth;

        m_totalUnbinds = 1;
    }

    bool WrathOfKhorneBloodthirster::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_bloodflail);
        model->addMissileWeapon(&m_breath);
        model->addMeleeWeapon(&m_mightyAxeOfKhorne);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *WrathOfKhorneBloodthirster::Create(const ParameterList &parameters) {
        auto unit = new WrathOfKhorneBloodthirster();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void WrathOfKhorneBloodthirster::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    WrathOfKhorneBloodthirster::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    WrathOfKhorneBloodthirster::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Wrath of Khorne Bloodthirster", factoryMethod);
        }
    }

    void WrathOfKhorneBloodthirster::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_mightyAxeOfKhorne.setToWound(g_damageTable[damageIndex].m_axeToWound);
        m_bloodflail.setDamage(g_damageTable[damageIndex].m_flailDamage);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        KhorneBase::onWounded();
    }

    int WrathOfKhorneBloodthirster::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Rerolls WrathOfKhorneBloodthirster::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Relentless Hunter
        if (target->hasKeyword(HERO) || target->hasKeyword(MONSTER)) {
            return RerollFailed;
        }
        return KhorneBase::toHitRerolls(weapon, target);
    }

    void WrathOfKhorneBloodthirster::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    Wounds WrathOfKhorneBloodthirster::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll,
                                                    int woundRoll) const {
        // Hellfire Breath
        if (weapon->name() == m_breath.name()) {
            if (Dice::rollD6() >= 2) {
                return {0, Dice::rollD3()};
            }
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int WrathOfKhorneBloodthirster::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Khorne