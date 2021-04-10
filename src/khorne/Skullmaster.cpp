/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Skullmaster.h>
#include <UnitFactory.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 75; // x50 oval
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 120;

    bool Skullmaster::s_registered = false;

    Skullmaster::Skullmaster(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral) :
            KhorneBase("Skullmaster", 8, g_wounds, 10, 4, false) {
        m_keywords = {CHAOS, DAEMON, BLOODLETTER, KHORNE, HERO, HERALD_OF_KHORNE, SKULLMASTER};
        m_weapons = {&m_bladeOfBlood, &m_brazenHooves};
        m_battleFieldRole = Role::Leader;

        setSlaughterHost(host);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_bladeOfBlood);
        model->addMeleeWeapon(&m_brazenHooves);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *Skullmaster::Create(const ParameterList &parameters) {
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new Skullmaster(host, trait, artefact, general);
    }

    void Skullmaster::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Skullmaster::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    Skullmaster::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Skullmaster", factoryMethod);
        }
    }

    Wounds Skullmaster::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Decapitating Blow
        if (hitRoll == 6) {
            return {weapon->damage(), 1};
        }
        return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void Skullmaster::onCharged() {
        // Murderous Charge
        if (m_meleeTarget && (distanceTo(m_meleeTarget) <= 1.0)) {
            Dice::RollResult rolls;
            Dice::RollD6(remainingModels(), rolls);
            Wounds wounds = {0, 0};
            if (remainingModels() >= 6) {
                for (int i = 0; i < rolls.rollsGE(2); i++) {
                    wounds.mortal += Dice::RollD3();
                }
            } else {
                wounds.mortal = rolls.rollsGE(2);
            }

            PLOG_INFO.printf("%s Murderous Charge inflicted %d mortal wounds on %s\n",
                             name().c_str(), wounds.mortal, m_meleeTarget->name().c_str());

            m_meleeTarget->applyDamage(wounds, this);
        }
        KhorneBase::onCharged();
    }

    Rerolls Skullmaster::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Slaughter and Ruin
        if (m_charged) {
            return Rerolls::Failed;
        }
        return KhorneBase::toHitRerolls(weapon, target);
    }

    int Skullmaster::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Khorne
