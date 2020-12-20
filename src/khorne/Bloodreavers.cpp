/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <khorne/Bloodreavers.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = 240;

    bool Bloodreavers::s_registered = false;

    Bloodreavers::Bloodreavers() :
            KhorneBase("Bloodreavers", 6, g_wounds, 5, 6, false),
            m_reaverBlades(Weapon::Type::Melee, "Reaver Blades", 1, 1, 4, 4, 0, 1),
            m_reaverBladesChieftain(Weapon::Type::Melee, "Reaver Blades", 1, 2, 4, 4, 0, 1),
            m_meatripperAxe(Weapon::Type::Melee, "Meatripper Axe", 1, 1, 4, 4, -1, 1),
            m_meatripperAxeChieftain(Weapon::Type::Melee, "Meatripper Axe", 1, 2, 4, 4, -1, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, BLOODREAVERS};
        m_weapons = {&m_reaverBlades, &m_reaverBladesChieftain, &m_meatripperAxe, &m_meatripperAxeChieftain};
        m_battleFieldRole = Battleline;
    }


    bool Bloodreavers::configure(int numModels, Bloodreavers::WeaponOption weapons, bool iconBearer, bool hornblowers) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_weaponOption = weapons;

        // Add the Chieftain
        auto chieftainModel = new Model(g_basesize, wounds());
        if (m_weaponOption == Reaver_Blades) {
            chieftainModel->addMeleeWeapon(&m_reaverBladesChieftain);
        } else if (m_weaponOption == Meatripper_Axe) {
            chieftainModel->addMeleeWeapon(&m_meatripperAxeChieftain);
        }
        addModel(chieftainModel);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (m_weaponOption == Reaver_Blades) {
                model->addMeleeWeapon(&m_reaverBlades);
            } else if (m_weaponOption == Meatripper_Axe) {
                model->addMeleeWeapon(&m_meatripperAxe);
            }
            if (iconBearer) {
                model->setName(Model::IconBearer);
                iconBearer = false;
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

    Rerolls Bloodreavers::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Reaver Blades
        if (m_weaponOption == Reaver_Blades) {
            return Reroll_Ones;
        }

        return KhorneBase::toHitRerolls(weapon, unit);
    }

    int Bloodreavers::braveryModifier() const {
        int modifier = KhorneBase::braveryModifier();

        // Icon Bearer
        if (isNamedModelAlive(Model::IconBearer)) {
            modifier++;
        }

        return modifier;
    }

    Unit *Bloodreavers::Create(const ParameterList &parameters) {
        auto unit = new Bloodreavers();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Reaver_Blades);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        bool ok = unit->configure(numModels, weapons, iconBearer, hornblowers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Bloodreavers::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Reaver_Blades, Meatripper_Axe};
            static FactoryMethod factoryMethod = {
                    Bloodreavers::Create,
                    Bloodreavers::ValueToString,
                    Bloodreavers::EnumStringToInt,
                    Bloodreavers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Reaver_Blades, weapons),
                            BoolParameter("Icon Bearer"),
                            BoolParameter("Hornblowers"),
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost)
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Bloodreavers", factoryMethod);
        }
    }

    std::string Bloodreavers::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Reaver_Blades) { return "Reaver Blades"; }
            else if (parameter.intValue == Meatripper_Axe) { return "Meatripper Axe"; }
        }
        return KhorneBase::ValueToString(parameter);
    }

    int Bloodreavers::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Reaver Blades") { return Reaver_Blades; }
        else if (enumString == "Meatripper Axe") { return Meatripper_Axe; }
        return KhorneBase::EnumStringToInt(enumString);
    }

    int Bloodreavers::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = KhorneBase::extraAttacks(attackingModel, weapon, target);

        // Frenzied Devotion
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 16.0);
        for (auto ip : units) {
            if (ip->hasKeyword(KHORNE) && ip->hasKeyword(TOTEM)) {
                attacks += 1;
                break;
            }
        }

        return attacks;
    }

    int Bloodreavers::runModifier() const {
        int modifier = KhorneBase::runModifier();
        if (isNamedModelAlive(Model::Hornblower)) {
            modifier += 1;
        }
        return modifier;
    }

    int Bloodreavers::chargeModifier() const {
        int modifier = KhorneBase::chargeModifier();
        if (isNamedModelAlive(Model::Hornblower)) {
            modifier += 1;
        }
        return modifier;
    }

    int Bloodreavers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Khorne