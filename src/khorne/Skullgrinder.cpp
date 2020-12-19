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

    Skullgrinder::Skullgrinder() :
            KhorneBase("Skullgrinder", 5, g_wounds, 8, 4, false),
            m_brazenAnvil(Weapon::Type::Melee, "Brazen Anvil", 2, 3, 3, 2, -1, 3) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, SKULLGRINDER};
        m_weapons = {&m_brazenAnvil};
        m_battleFieldRole = Leader;

        s_globalBraveryMod.connect(this, &Skullgrinder::favouredByKhorne, &m_connection);
    }

    Skullgrinder::~Skullgrinder() {
        m_connection.disconnect();
    }

    bool Skullgrinder::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_brazenAnvil);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *Skullgrinder::Create(const ParameterList &parameters) {
        auto unit = new Skullgrinder();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

    Wounds Skullgrinder::onEndCombat(PlayerId player) {
        auto wounds = Unit::onEndCombat(player);

        // Fiery Anvil
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 2.0);
        for (auto unit : units) {
            if (unit->hasKeyword(HERO) || unit->hasKeyword(MONSTER)) {
                if (Dice::RollD6() >= 2) {
                    Wounds anvilWounds = {0, Dice::RollD3()};
                    unit->applyDamage(anvilWounds, this);
                    wounds += anvilWounds;
                    break;
                }
            }
        }
        return wounds;
    }

    int Skullgrinder::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }


} // namespace Khorne