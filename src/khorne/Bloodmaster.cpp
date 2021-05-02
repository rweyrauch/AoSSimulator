/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Bloodmaster.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 80;

    bool Bloodmaster::s_registered = false;

    Bloodmaster::Bloodmaster(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral) :
            KhorneBase("Bloodmaster", 5, g_wounds, 10, 4, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, DAEMON, BLOODLETTER, KHORNE, HERO, HERALD_OF_KHORNE, BLOODMASTER};
        m_weapons = {&m_bladeOfBlood};
        m_battleFieldRole = Role::Leader;

        setSlaughterHost(host);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_bladeOfBlood);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *Bloodmaster::Create(const ParameterList &parameters) {
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new Bloodmaster(host, trait, artefact, general);
    }

    void Bloodmaster::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Bloodmaster::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    Bloodmaster::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Bloodmaster", factoryMethod);
        }
    }

    Wounds Bloodmaster::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Decapitating Blow
        if (hitRoll == 6) {
            return {weapon->damage(), 1};
        }
        return KhorneBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void Bloodmaster::onEndCombat(PlayerId player) {
        if (!m_usedBloodMustFlow) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
            for (auto unit : units) {
                if ((unit->meleeTarget() != nullptr) && (unit->distanceTo(unit->meleeTarget()) <= 3.0) &&
                    unit->hasKeyword(BLOODLETTERS) && !unit->hasFought()) {
                    int numSlain;
                    unit->fight(player, numSlain);
                    m_usedBloodMustFlow = true;
                }
            }
        }
        KhorneBase::onEndCombat(player);
    }

    int Bloodmaster::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void Bloodmaster::onStartCombat(PlayerId player) {
        KhorneBase::onStartCombat(player);
        m_usedBloodMustFlow = false;
    }


} // namespace Khorne