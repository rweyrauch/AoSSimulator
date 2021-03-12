/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/BloodThrone.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 120;

    bool HeraldOfKhorneOnBloodThrone::s_registered = false;

    HeraldOfKhorneOnBloodThrone::HeraldOfKhorneOnBloodThrone() :
            KhorneBase("Herald of Khorne on Blood Throne", 8, g_wounds, 10, 4, false),
            m_bladeOfBlood(Weapon::Type::Melee, "Blade of Blood", 1, 4, 3, 3, -1, 1),
            m_hellblades(Weapon::Type::Melee, "Hellblades", 1, 2, 4, 3, -1, 1),
            m_gnashingMaw(Weapon::Type::Melee, "Gnashing Maw", 1, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, KHORNE, BLOODLETTER, HERO, HERALD_OF_KHORNE, HERALD_OF_KHORNE_ON_BLOOD_THRONE};
        m_weapons = {&m_bladeOfBlood, &m_hellblades, &m_gnashingMaw};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_gnashingMaw.setMount(true);
    }

    bool HeraldOfKhorneOnBloodThrone::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_bladeOfBlood);
        model->addMeleeWeapon(&m_hellblades);
        model->addMeleeWeapon(&m_gnashingMaw);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *HeraldOfKhorneOnBloodThrone::Create(const ParameterList &parameters) {
        auto unit = new HeraldOfKhorneOnBloodThrone();

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

    Wounds HeraldOfKhorneOnBloodThrone::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll,
                                                     int woundRoll) const {
        // Decapitating Blow
        if (hitRoll == 6) {
            return {weapon->damage(), 1};
        }
        return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int HeraldOfKhorneOnBloodThrone::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Khorne