/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/BloodThrone.h>
#include <UnitFactory.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 120;

    bool HeraldOfKhorneOnBloodThrone::s_registered = false;

    HeraldOfKhorneOnBloodThrone::HeraldOfKhorneOnBloodThrone(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral) :
            KhorneBase("Herald of Khorne on Blood Throne", 8, g_wounds, 10, 4, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, DAEMON, KHORNE, BLOODLETTER, HERO, HERALD_OF_KHORNE, HERALD_OF_KHORNE_ON_BLOOD_THRONE};
        m_weapons = {&m_bladeOfBlood, &m_hellblades, &m_gnashingMaw};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_gnashingMaw.setMount(true);

        setSlaughterHost(host);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_bladeOfBlood);
        model->addMeleeWeapon(&m_hellblades);
        model->addMeleeWeapon(&m_gnashingMaw);
        addModel(model);

        m_commandAbilities.push_back(std::unique_ptr<CommandAbility>(CreateAtTheDouble(this, 12, 12)));
        m_commandAbilities.push_back(std::unique_ptr<CommandAbility>(CreateInspiringPresence(this, 12, 12)));
        m_commandAbilities.push_back(std::unique_ptr<CommandAbility>(CreateForwardToVictory(this, 12, 12)));

        m_points = g_pointsPerUnit;
    }

    Unit *HeraldOfKhorneOnBloodThrone::Create(const ParameterList &parameters) {
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new HeraldOfKhorneOnBloodThrone(host, trait, artefact, general);
    }

    void HeraldOfKhorneOnBloodThrone::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    HeraldOfKhorneOnBloodThrone::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    HeraldOfKhorneOnBloodThrone::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Herald of Khorne on Blood Throne", factoryMethod);
        }
    }

    Wounds HeraldOfKhorneOnBloodThrone::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll,
                                                     int woundRoll) const {
        // Decapitating Blow
        if (hitRoll == 6) {
            return {weapon->damage(), 1};
        }
        return KhorneBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int HeraldOfKhorneOnBloodThrone::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void HeraldOfKhorneOnBloodThrone::onEnemyModelWoundedWithWeapon(Unit *enemyUnit, const Weapon *weapon,
                                                                    const Wounds &weaponDamage) {
        KhorneBase::onEnemyModelWoundedWithWeapon(enemyUnit, weapon, weaponDamage);

        // Gorefeast
        if (weapon->name() == m_gnashingMaw.name()) {
            heal(Dice::RollD3());
        }
    }

} // namespace Khorne