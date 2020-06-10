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

namespace Khorne {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 240;

    bool Bloodreavers::s_registered = false;

    Bloodreavers::Bloodreavers() :
            KhorneBase("Bloodreavers", 6, WOUNDS, 5, 6, false),
            m_reaverBlades(Weapon::Type::Melee, "Reaver Blades", 1, 1, 4, 4, 0, 1),
            m_reaverBladesChieftain(Weapon::Type::Melee, "Reaver Blades", 1, 2, 4, 4, 0, 1),
            m_meatripperAxe(Weapon::Type::Melee, "Meatripper Axe", 1, 1, 4, 4, -1, 1),
            m_meatripperAxeChieftain(Weapon::Type::Melee, "Meatripper Axe", 1, 2, 4, 4, -1, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, BLOODREAVERS};
        m_weapons = {&m_reaverBlades, &m_reaverBladesChieftain, &m_meatripperAxe, &m_meatripperAxeChieftain};
    }


    bool Bloodreavers::configure(int numModels, Bloodreavers::WeaponOption weapons, bool iconBearer, bool hornblowers) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_weaponOption = weapons;
        m_iconBearer = iconBearer;
        m_hornblower = hornblowers;

        // Add the Chieftain
        auto chieftainModel = new Model(BASESIZE, wounds());
        if (m_weaponOption == ReaverBlades) {
            chieftainModel->addMeleeWeapon(&m_reaverBladesChieftain);
        } else if (m_weaponOption == MeatripperAxe) {
            chieftainModel->addMeleeWeapon(&m_meatripperAxeChieftain);
        }
        addModel(chieftainModel);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (m_weaponOption == ReaverBlades) {
                model->addMeleeWeapon(&m_reaverBlades);
            } else if (m_weaponOption == MeatripperAxe) {
                model->addMeleeWeapon(&m_meatripperAxe);
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Rerolls Bloodreavers::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Reaver Blades
        if (m_weaponOption == ReaverBlades) {
            return RerollOnes;
        }

        return KhorneBase::toHitRerolls(weapon, unit);
    }

    int Bloodreavers::braveryModifier() const {
        int modifier = KhorneBase::braveryModifier();

        // Icon Bearer
        if (m_iconBearer) {
            modifier++;
        }

        return modifier;
    }

    Unit *Bloodreavers::Create(const ParameterList &parameters) {
        auto unit = new Bloodreavers();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, ReaverBlades);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, Khorne::None);
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
            static const std::array<int, 2> weapons = {ReaverBlades, MeatripperAxe};
            static FactoryMethod factoryMethod = {
                    Bloodreavers::Create,
                    Bloodreavers::ValueToString,
                    Bloodreavers::EnumStringToInt,
                    Bloodreavers::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Weapons", ReaverBlades, weapons),
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
            if (parameter.intValue == ReaverBlades) { return "Reaver Blades"; }
            else if (parameter.intValue == MeatripperAxe) { return "Meatripper Axe"; }
        }
        return KhorneBase::ValueToString(parameter);
    }

    int Bloodreavers::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Reaver Blades") { return ReaverBlades; }
        else if (enumString == "Meatripper Axe") { return MeatripperAxe; }
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
        if (m_hornblower) {
            modifier += 1;
        }
        return modifier;
    }

    int Bloodreavers::chargeModifier() const {
        int modifier = KhorneBase::chargeModifier();
        if (m_hornblower) {
            modifier += 1;
        }
        return modifier;
    }

    int Bloodreavers::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace Khorne