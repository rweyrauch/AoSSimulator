/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Skullgrinder.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {

    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 80;

    bool Skullgrinder::s_registered = false;

    Skullgrinder::Skullgrinder(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral) :
            KhorneBase("Skullgrinder", 5, g_wounds, 8, 4, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, SKULLGRINDER};
        m_weapons = {&m_brazenAnvil};
        m_battleFieldRole = Role::Leader;

        s_globalBraveryMod.connect(this, &Skullgrinder::favouredByKhorne, &m_connection);

        setSlaughterHost(host);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_brazenAnvil);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Skullgrinder::~Skullgrinder() {
        m_connection.disconnect();
    }

    Unit *Skullgrinder::Create(const ParameterList &parameters) {
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortalbloodboundCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_mortalArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new Skullgrinder(host, trait, artefact, general);
    }

    void Skullgrinder::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            EnumParameter("Command Trait", g_mortalbloodboundCommandTraits[0],
                                          g_mortalbloodboundCommandTraits),
                            EnumParameter("Artefact", g_mortalArtefacts[0], g_mortalArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Skullgrinder", factoryMethod);
        }
    }

    std::string Skullgrinder::ValueToString(const Parameter &parameter) {
        return KhorneBase::ValueToString(parameter);
    }

    int Skullgrinder::EnumStringToInt(const std::string &enumString) {
        return KhorneBase::EnumStringToInt(enumString);
    }

    int Skullgrinder::favouredByKhorne(const Unit *unit) {
        // Favoured by Khorne
        if (unit->hasKeyword(KHORNE) && unit->hasKeyword(MORTAL) &&
            (unit->owningPlayer() == owningPlayer()) && (distanceTo(unit) <= 12.0)) {
            return 1;
        }
        return 0;
    }

    void Skullgrinder::onEndCombat(PlayerId player) {
        KhorneBase::onEndCombat(player);

        // Fiery Anvil
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 2.0);
        for (auto unit : units) {
            if (unit->hasKeyword(HERO) || unit->hasKeyword(MONSTER)) {
                if (Dice::RollD6() >= 2) {
                    Wounds anvilWounds = {0, Dice::RollD3()};
                    unit->applyDamage(anvilWounds, this);
                    break;
                }
            }
        }
    }

    int Skullgrinder::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }


} // namespace Khorne