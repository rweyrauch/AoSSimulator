/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Bloodsecrator.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool Bloodsecrator::s_registered = false;

    Bloodsecrator::Bloodsecrator() :
            KhorneBase("Bloodsecrator", 4, g_wounds, 9, 3, false),
            m_ensorcelledAxe(Weapon::Type::Melee, "Ensorcelled Axe", 1, 4, 3, 3, -1, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, TOTEM, BLOODSECRATOR};
        m_weapons = {&m_ensorcelledAxe};
        m_battleFieldRole = Leader;

        s_globalAttackMod.connect(this, &Bloodsecrator::rageOfKhorneAttackMod, &m_rageOfKhorneSlot);
    }

    Bloodsecrator::~Bloodsecrator() {
        m_rageOfKhorneSlot.disconnect();
    }

    bool Bloodsecrator::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_ensorcelledAxe);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *Bloodsecrator::Create(const ParameterList &parameters) {
        auto unit = new Bloodsecrator();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortalbloodboundCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_totemArtefacts[0]);
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

    void Bloodsecrator::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Bloodsecrator::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    Bloodsecrator::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            EnumParameter("Command Trait", g_mortalbloodboundCommandTraits[0], g_mortalbloodboundCommandTraits),
                            EnumParameter("Artefact", g_totemArtefacts[0], g_totemArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Bloodsecrator", factoryMethod);
        }
    }

    int Bloodsecrator::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    int
    Bloodsecrator::rageOfKhorneAttackMod(const Unit *attacker, const Model * /*attackingModel*/, const Weapon *weapon,
                                         const Unit * /*target*/) {
        if (isFriendly(attacker) && attacker->hasKeyword(KHORNE) && !weapon->isMissile() &&
            (distanceTo(attacker) <= 16.0))
            return 1;
        return 0;
    }

} // namespace Khorne