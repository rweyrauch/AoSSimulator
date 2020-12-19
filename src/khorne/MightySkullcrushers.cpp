/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/MightySkullcrushers.h>
#include <UnitFactory.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 5;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 160;
    static const int g_pointsMaxUnitSize = 160 * 4;

    bool MightySkullcrushers::s_registered = false;

    MightySkullcrushers::MightySkullcrushers() :
            KhorneBase("Mighty Skullcrushers", 8, g_wounds, 6, 3, false),
            m_ensorcelledAxe(Weapon::Type::Melee, "Ensorcelled Axe", 1, 3, 3, 3, 0, 1),
            m_bloodglaive(Weapon::Type::Melee, "Bloodglaive", 1, 3, 4, 3, -1, 1),
            m_ensorcelledAxeHunter(Weapon::Type::Melee, "Ensorcelled Axe", 1, 4, 3, 3, 0, 1),
            m_bloodglaiveHunter(Weapon::Type::Melee, "Bloodglaive", 1, 4, 4, 3, -1, 1),
            m_brazenHooves(Weapon::Type::Melee, "Brazen Hooves", 1, 3, 3, 3, 0, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, MIGHTY_SKULLCRUSHERS};
        m_weapons = {&m_ensorcelledAxe, &m_bloodglaive, &m_ensorcelledAxeHunter, &m_bloodglaiveHunter, &m_brazenHooves};
        m_hasMount = true;
    }

    bool MightySkullcrushers::configure(int numModels, WeaponOption weapons, bool standardBearer, bool hornblowers) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_weaponOption = weapons;

        // Add the Skullhunter
        auto hunter = new Model(g_basesize, wounds());
        if (m_weaponOption == Ensorcelled_Axe) {
            hunter->addMeleeWeapon(&m_ensorcelledAxeHunter);
        } else if (m_weaponOption == Bloodglaive) {
            hunter->addMeleeWeapon(&m_bloodglaiveHunter);
        }
        hunter->addMeleeWeapon(&m_brazenHooves);
        addModel(hunter);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (m_weaponOption == Ensorcelled_Axe) {
                model->addMeleeWeapon(&m_ensorcelledAxe);
            } else if (m_weaponOption == Bloodglaive) {
                model->addMeleeWeapon(&m_bloodglaive);
            }
            model->addMeleeWeapon(&m_brazenHooves);
            if (standardBearer) {
                model->setName("Standard Bearer");
                standardBearer = false;
            }
            else if (hornblowers) {
                model->setName("Hornblower");
                hornblowers = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *MightySkullcrushers::Create(const ParameterList &parameters) {
        auto unit = new MightySkullcrushers();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Ensorcelled_Axe);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        bool ok = unit->configure(numModels, weapons, standardBearer, hornblowers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void MightySkullcrushers::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Ensorcelled_Axe, Bloodglaive};
            static FactoryMethod factoryMethod = {
                    MightySkullcrushers::Create,
                    MightySkullcrushers::ValueToString,
                    MightySkullcrushers::EnumStringToInt,
                    MightySkullcrushers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Ensorcelled_Axe, weapons),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblowers"),
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost)
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Mighty Skullcrushers", factoryMethod);
        }
    }

    int MightySkullcrushers::chargeModifier() const {
        int modifier = KhorneBase::chargeModifier();
        if (isNamedModelAlive("Hornblower"))
            modifier += 1;
        return modifier;
    }

    int MightySkullcrushers::runModifier() const {
        int modifier = KhorneBase::runModifier();
        if (isNamedModelAlive("Hornblower"))
            modifier += 1;
        return modifier;
    }

    int MightySkullcrushers::braveryModifier() const {
        int modifier = KhorneBase::braveryModifier();
        if (isNamedModelAlive("Standard Bearer"))
            modifier += 2;
        return modifier;
    }

    std::string MightySkullcrushers::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Ensorcelled_Axe) { return "Ensorcelled Axe"; }
            else if (parameter.intValue == Bloodglaive) { return "Bloodglaive"; }
        }
        return KhorneBase::ValueToString(parameter);
    }

    int MightySkullcrushers::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ensorcelled Axe") { return Ensorcelled_Axe; }
        else if (enumString == "Bloodglaive") { return Bloodglaive; }
        return KhorneBase::EnumStringToInt(enumString);
    }

    void MightySkullcrushers::onCharged() {
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

            SimLog(Verbosity::Narrative, "%s Murderous Charge inflicted %d mortal wounds on %s\n",
                   name().c_str(), wounds.mortal, m_meleeTarget->name().c_str());

            m_meleeTarget->applyDamage(wounds, this);
        }
        KhorneBase::onCharged();
    }

    int MightySkullcrushers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Khorne
