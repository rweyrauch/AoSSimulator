/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/MightySkullcrushers.h>
#include <UnitFactory.h>

namespace Khorne {
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = 160 * 4;

    bool MightySkullcrushers::s_registered = false;

    MightySkullcrushers::MightySkullcrushers() :
            KhorneBase("Mighty Skullcrushers", 8, WOUNDS, 6, 3, false),
            m_ensorcelledAxe(Weapon::Type::Melee, "Ensorcelled Axe", 1, 3, 3, 3, 0, 1),
            m_bloodglaive(Weapon::Type::Melee, "Bloodglaive", 1, 3, 4, 3, -1, 1),
            m_ensorcelledAxeHunter(Weapon::Type::Melee, "Ensorcelled Axe", 1, 4, 3, 3, 0, 1),
            m_bloodglaiveHunter(Weapon::Type::Melee, "Bloodglaive", 1, 4, 4, 3, -1, 1),
            m_brazenHooves(Weapon::Type::Melee, "Brazen Hooves", 1, 3, 3, 3, 0, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, MIGHTY_SKULLCRUSHERS};
        m_weapons = {&m_ensorcelledAxe, &m_bloodglaive, &m_ensorcelledAxeHunter, &m_bloodglaiveHunter, &m_brazenHooves};
    }

    bool MightySkullcrushers::configure(int numModels, WeaponOption weapons, bool standardBearer, bool hornblowers) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_weaponOption = weapons;
        m_standarBearer = standardBearer;
        m_hornblower = hornblowers;

        // Add the Skullhunter
        auto hunter = new Model(BASESIZE, wounds());
        if (m_weaponOption == EnsorcelledAxe) {
            hunter->addMeleeWeapon(&m_ensorcelledAxeHunter);
        } else if (m_weaponOption == Bloodglaive) {
            hunter->addMeleeWeapon(&m_bloodglaiveHunter);
        }
        hunter->addMeleeWeapon(&m_brazenHooves);
        addModel(hunter);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (m_weaponOption == EnsorcelledAxe) {
                model->addMeleeWeapon(&m_ensorcelledAxe);
            } else if (m_weaponOption == Bloodglaive) {
                model->addMeleeWeapon(&m_bloodglaive);
            }
            model->addMeleeWeapon(&m_brazenHooves);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *MightySkullcrushers::Create(const ParameterList &parameters) {
        auto unit = new MightySkullcrushers();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, EnsorcelledAxe);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, Khorne::None);
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
            static const std::array<int, 2> weapons = {EnsorcelledAxe, Bloodglaive};
            static FactoryMethod factoryMethod = {
                    MightySkullcrushers::Create,
                    MightySkullcrushers::ValueToString,
                    MightySkullcrushers::EnumStringToInt,
                    MightySkullcrushers::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Weapons", EnsorcelledAxe, weapons),
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
        if (m_hornblower)
            modifier += 1;
        return modifier;
    }

    int MightySkullcrushers::runModifier() const {
        int modifier = KhorneBase::runModifier();
        if (m_hornblower)
            modifier += 1;
        return modifier;
    }

    int MightySkullcrushers::braveryModifier() const {
        int modifier = KhorneBase::braveryModifier();
        if (m_standarBearer)
            modifier += 2;
        return modifier;
    }

    std::string MightySkullcrushers::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == EnsorcelledAxe) { return "Ensorcelled Axe"; }
            else if (parameter.intValue == Bloodglaive) { return "Bloodglaive"; }
        }
        return KhorneBase::ValueToString(parameter);
    }

    int MightySkullcrushers::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ensorcelled Axe") { return EnsorcelledAxe; }
        else if (enumString == "Bloodglaive") { return Bloodglaive; }
        return KhorneBase::EnumStringToInt(enumString);
    }

    void MightySkullcrushers::onCharged() {
        // Murderous Charge
        if (m_meleeTarget && (distanceTo(m_meleeTarget) <= 1.0)) {
            Dice::RollResult rolls;
            Dice::rollD6(remainingModels(), rolls);
            Wounds wounds = {0, 0};
            if (remainingModels() >= 6) {
                for (int i = 0; i < rolls.rollsGE(2); i++) {
                    wounds.mortal += Dice::rollD3();
                }
            } else {
                wounds.mortal = rolls.rollsGE(2);
            }

            SimLog(Verbosity::Narrative, "%s Murderous Charge inflicted %d mortal wounds on %s\n",
                   name().c_str(), wounds.mortal, m_meleeTarget->name().c_str());

            m_meleeTarget->applyDamage(wounds);
        }
        KhorneBase::onCharged();
    }

    int MightySkullcrushers::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace Khorne
