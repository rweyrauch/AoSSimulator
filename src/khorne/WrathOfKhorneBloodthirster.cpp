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
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 300;

    struct TableEntry {
        int m_move;
        int m_flailDamage;
        int m_axeToWound;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 6,       2},
                    {9,  3,       3},
                    {8,  3,       3},
                    {7,  RAND_D3, 4},
                    {6,  RAND_D3, 4}
            };

    bool WrathOfKhorneBloodthirster::s_registered = false;

    WrathOfKhorneBloodthirster::WrathOfKhorneBloodthirster(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral) :
            KhorneBase("Wrath of Khorne Bloodthirster", 10, g_wounds, 10, 4, true, g_pointsPerUnit) {
        m_keywords = {CHAOS, DAEMON, BLOODTHIRSTER, KHORNE, MONSTER, HERO, WRATH_OF_KHORNE_BLOODTHIRSTER};
        m_weapons = {&m_bloodflail, &m_mightyAxeOfKhorne, &m_breath};
        m_battleFieldRole = Role::Leader_Behemoth;

        m_totalUnbinds = 1;

        setSlaughterHost(host);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_bloodflail);
        model->addMissileWeapon(&m_breath);
        model->addMeleeWeapon(&m_mightyAxeOfKhorne);
        addModel(model);

        m_commandAbilities.push_back(std::make_unique<BuffRerollCommandAbility>(this, "Lord of the Blood Hunt", 16, 16,
                                                                                GamePhase::Combat, Attribute::To_Hit_Melee,
                                                                                Rerolls::Failed,
                                                                                Abilities::Target::SelfAndFriendly,
                                                                                std::vector<Keyword>{KHORNE, DAEMON}));

        m_points = g_pointsPerUnit;
    }

    Unit *WrathOfKhorneBloodthirster::Create(const ParameterList &parameters) {
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new WrathOfKhorneBloodthirster(host, trait, artefact, general);
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
        const auto damageIndex = getDamageTableIndex();
        m_mightyAxeOfKhorne.setToWound(g_damageTable[damageIndex].m_axeToWound);
        m_bloodflail.setDamage(g_damageTable[damageIndex].m_flailDamage);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        KhorneBase::onWounded();
    }

    size_t WrathOfKhorneBloodthirster::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Rerolls WrathOfKhorneBloodthirster::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Relentless Hunter
        if (target->hasKeyword(HERO) || target->hasKeyword(MONSTER)) {
            return Rerolls::Failed;
        }
        return KhorneBase::toHitRerolls(weapon, target);
    }

    void WrathOfKhorneBloodthirster::onRestore() {
        // Restore table-driven attributes
        onWounded();
        KhorneBase::onRestore();
    }

    Wounds WrathOfKhorneBloodthirster::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll,
                                                    int woundRoll) const {
        // Hellfire Breath
        if (weapon->name() == m_breath.name()) {
            if (Dice::RollD6() >= 2) {
                return {0, Dice::RollD3(), Wounds::Source::Ability, nullptr};
            }
        }
        return KhorneBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int WrathOfKhorneBloodthirster::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Khorne