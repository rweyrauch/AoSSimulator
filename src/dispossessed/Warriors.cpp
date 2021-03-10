/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/Warriors.h>
#include <UnitFactory.h>
#include "DispossessedPrivate.h"

namespace Dispossessed {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 280;

    bool Warriors::s_registered = false;

    Warriors::Warriors() :
            Dispossessed("Warriors", 4, g_wounds, 6, 5, false),
            m_duardinAxeOrHammer(Weapon::Type::Melee, "Duardin Axe or Duardin Hammer", 1, 1, 3, 4, 0, 1),
            m_duardinAxeOrHammerVeteran(Weapon::Type::Melee, "Duardin Axe or Duardin Hammer", 1, 2, 3, 4, 0, 1),
            m_doubleHandedAxe(Weapon::Type::Melee, "Double-handed Duardin Axe", 1, 1, 4, 3, -1, 1),
            m_doubleHandedAxeVeteran(Weapon::Type::Melee, "Double-handed Duardin Axe", 1, 2, 4, 3, -1, 1) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, WARRIORS};
        m_weapons = {&m_duardinAxeOrHammer, &m_duardinAxeOrHammerVeteran, &m_doubleHandedAxe,
                     &m_doubleHandedAxeVeteran};
    }

    bool Warriors::configure(int numModels, WeaponOptions weapons, bool duardinShields, StandardOptions standard,
                             bool hornblowers) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_duardinShields = duardinShields;

        auto veteran = new Model(g_basesize, wounds());
        if (weapons == Duardin_Axe_Or_Hammer) {
            veteran->addMeleeWeapon(&m_duardinAxeOrHammerVeteran);
        } else if (weapons == Double_Handed_Duardin_Axe) {
            veteran->addMeleeWeapon(&m_doubleHandedAxeVeteran);
        }
        addModel(veteran);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Duardin_Axe_Or_Hammer) {
                model->addMeleeWeapon(&m_duardinAxeOrHammer);
            } else if (weapons == Double_Handed_Duardin_Axe) {
                model->addMeleeWeapon(&m_doubleHandedAxe);
            }
            if (standard == StandardOptions::Clan_Banner) {
                model->setName("Clan Banner");
                standard = StandardOptions::None;
            }
            else if (standard == StandardOptions::Runic_Icon) {
                model->setName("Runic Icon");
                standard = StandardOptions::None;
            }
            else if (hornblowers) {
                model->setName(Model::Hornblower);
                hornblowers = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Warriors::Create(const ParameterList &parameters) {
        auto unit = new Warriors();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, Duardin_Axe_Or_Hammer);
        bool duardinShields = GetBoolParam("Duardin Shields", parameters, false);
        auto standard = (StandardOptions) GetEnumParam("Standard", parameters, None);
        bool hornblower = GetBoolParam("Hornblower", parameters, false);

        bool ok = unit->configure(numModels, weapons, duardinShields, standard, hornblower);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Warriors::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Warriors::Duardin_Axe_Or_Hammer, Warriors::Double_Handed_Duardin_Axe};
            static FactoryMethod factoryMethod = {
                    Warriors::Create,
                    Warriors::ValueToString,
                    Warriors::EnumStringToInt,
                    Warriors::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Warriors::Duardin_Axe_Or_Hammer, weapons),
                            BoolParameter("Duardin Shields"),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblowers"),
                            EnumParameter("Grudge", g_grudge[0], g_grudge)
                    },
                    ORDER,
                    {DISPOSSESSED}
            };
            s_registered = UnitFactory::Register("Warriors", factoryMethod);
        }
    }

    Rerolls Warriors::toSaveRerolls(const Weapon *weapon, const Unit* attacker) const {
        // Duardin Shields
        if (!m_ran && !m_charged) {
            if (!weapon->isMissile())
                return Reroll_Failed;
        }
        return Dispossessed::toSaveRerolls(weapon, attacker);
    }

    Rerolls Warriors::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Resolute in Defence
        if (m_opponentsCombat) {
            if (remainingModels() >= 20)
                return Reroll_Failed;
            else
                return Reroll_Ones;
        }
        return Dispossessed::toWoundRerolls(weapon, target);
    }

    void Warriors::onStartCombat(PlayerId player) {
        Dispossessed::onStartCombat(player);

        m_opponentsCombat = (player != owningPlayer());
    }

    std::string Warriors::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Duardin_Axe_Or_Hammer) {
                return "Duardin Axe Or Hammer";
            } else if (parameter.intValue == Double_Handed_Duardin_Axe) {
                return "Double Handed Duardin Axe";
            }
        } else if (std::string(parameter.name) == "Standard") {
            if (parameter.intValue == None) {
                return "None";
            } else if (parameter.intValue == Runic_Icon) {
                return "Runic Icon";
            } else if (parameter.intValue == Clan_Banner) {
                return "Clan Banner";
            }
        }

        return Dispossessed::ValueToString(parameter);
    }

    int Warriors::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Duardin Axe Or Hammer") {
            return Duardin_Axe_Or_Hammer;
        } else if (enumString == "Double Handed Duardin Axe") {
            return Double_Handed_Duardin_Axe;
        } else if (enumString == "None") {
            return None;
        } else if (enumString == "Runic Icon") {
            return Runic_Icon;
        } else if (enumString == "Clan Banner") {
            return Clan_Banner;
        }
        return Dispossessed::EnumStringToInt(enumString);
    }

    int Warriors::rollRunDistance() {
        // Sound the Advance
        if (isNamedModelAlive(Model::Hornblower)) {
            return 4;
        }
        return Unit::rollRunDistance();
    }

    void Warriors::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        Dispossessed::computeBattleshockEffect(roll, numFled, numAdded);

        if (isNamedModelAlive("Clan Banner")) {
            numFled = (numFled + 1) / 2;
        }
    }

    int Warriors::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Dispossessed
