/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/LordOfKhorneOnJuggernaut.h>
#include <UnitFactory.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 160;

    bool LordOfKhorneOnJuggernaut::s_registered = false;

    LordOfKhorneOnJuggernaut::LordOfKhorneOnJuggernaut() :
            KhorneBase("Lord of Khorne on Juggernaut", 5, WOUNDS, 9, 3, false),
            m_wrathforgedAxe(Weapon::Type::Melee, "Axe of Khorne", 1, 3, 3, 3, -1, RAND_D3),
            m_brazenHooves(Weapon::Type::Melee, "Brazen Hooves", 1, 3, 3, 3, 0, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, LORD_OF_KHORNE_ON_JUGGERNAUT};
        m_weapons = {&m_wrathforgedAxe, &m_brazenHooves};
        m_battleFieldRole = Leader;
        m_hasMount = true;
    }

    bool LordOfKhorneOnJuggernaut::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_wrathforgedAxe);
        model->addMeleeWeapon(&m_brazenHooves);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *LordOfKhorneOnJuggernaut::Create(const ParameterList &parameters) {
        auto unit = new LordOfKhorneOnJuggernaut();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortalbloodboundCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_mortalArtefacts[0]);
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

    void LordOfKhorneOnJuggernaut::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LordOfKhorneOnJuggernaut::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    LordOfKhorneOnJuggernaut::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            EnumParameter("Command Trait", g_mortalbloodboundCommandTraits[0], g_mortalbloodboundCommandTraits),
                            EnumParameter("Artefact", g_mortalArtefacts[0], g_mortalArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Lord of Khorne on Juggernaut", factoryMethod);
        }
    }

    void LordOfKhorneOnJuggernaut::onCharged() {
        // Slaughterous Charge
        if (m_meleeTarget && (distanceTo(m_meleeTarget) <= 1.0)) {
            int roll = Dice::rollD6();
            if (roll >= 2) {
                Wounds wounds = {0, Dice::rollD3()};

                SimLog(Verbosity::Narrative, "%s Murderous Charge inflicted %d mortal wounds on %s\n",
                       name().c_str(), wounds.mortal, m_meleeTarget->name().c_str());

                m_meleeTarget->applyDamage(wounds);
            }
        }
        KhorneBase::onCharged();
    }

    Wounds
    LordOfKhorneOnJuggernaut::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Daemonic Axe
        if ((hitRoll == 6) && (weapon->name() == m_wrathforgedAxe.name())) {
            return {RAND_D3, 0};
        }
        return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int LordOfKhorneOnJuggernaut::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    Wounds LordOfKhorneOnJuggernaut::applyWoundSave(const Wounds &wounds) {
        auto totalWounds = Unit::applyWoundSave(wounds);

        // Brass-clad Shield
        if (totalWounds.source == Wounds::Source::Spell) {
            Dice::RollResult result;
            Dice::rollD6(totalWounds.normal, result);
            totalWounds.normal -= result.rollsGE(5);
            Dice::rollD6(totalWounds.mortal, result);
            totalWounds.mortal -= result.rollsGE(5);
        }

        return totalWounds;
    }


} // namespace Khorne
