/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/MortekGuard.h"

namespace OssiarchBonereapers {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 130;
    static const int POINTS_MAX_UNIT_SIZE = 440;

    bool MortekGuard::s_registered = false;

    Unit *MortekGuard::Create(const ParameterList &parameters) {
        auto unit = new MortekGuard();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, NadiriteBladeAndShield);
        bool necrophoros = GetBoolParam("Necrophoros", parameters, true);
        int numGreatblades = GetIntParam("Soulcleaver Greatblade", parameters, 1);

        auto legion = (Legion) GetEnumParam("Legion", parameters, NoLegion);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels, weapons, numGreatblades, necrophoros);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string MortekGuard::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == NadirateSpearAndShield) {
                return "Nadirate Spear and Shield";
            } else if (parameter.intValue == NadiriteBladeAndShield) {
                return "Nadirite Blade and Shield";
            }
        }
        return OssiarchBonereaperBase::ValueToString(parameter);
    }

    int MortekGuard::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Nadirite Blade and Shield") return NadiriteBladeAndShield;
        else if (enumString == "Nadirate Spear and Shield") return NadirateSpearAndShield;

        return OssiarchBonereaperBase::EnumStringToInt(enumString);
    }

    void MortekGuard::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {NadiriteBladeAndShield, NadirateSpearAndShield};
            static FactoryMethod factoryMethod = {
                    MortekGuard::Create,
                    MortekGuard::ValueToString,
                    MortekGuard::EnumStringToInt,
                    MortekGuard::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            BoolParameter("Necrophoros"),
                            EnumParameter("Weapons", NadiriteBladeAndShield, weapons),
                            IntegerParameter("Soulcleaver Greatblade", 1, 0, MAX_UNIT_SIZE / 3, 1),
                            EnumParameter("Legion", g_legion[0], g_legion),
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Mortek Guard", factoryMethod);
        }
    }

    MortekGuard::MortekGuard() :
            OssiarchBonereaperBase("Mortek Guard", 4, WOUNDS, 10, 4, false),
            m_blade(Weapon::Type::Melee, "Nadirite Blade", 1, 2, 3, 4, -1, 1),
            m_spear(Weapon::Type::Melee, "Nadirite Spear", 2, 2, 3, 4, 0, 1),
            m_greatblade(Weapon::Type::Melee, "Soulcleaver Greatblade", 1, 2, 3, 3, -1, 1),
            m_bladeHekatos(Weapon::Type::Melee, "Nadirite Blade", 1, 3, 3, 4, -1, 1),
            m_spearHekatos(Weapon::Type::Melee, "Nadirite Spear", 2, 3, 3, 4, 0, 1),
            m_greatbladeHekatos(Weapon::Type::Melee, "Soulcleaver Greatblade", 1, 3, 3, 3, -1, 1) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, MORTEK_GUARD};
        m_weapons = {&m_blade, &m_spear, &m_greatblade, &m_bladeHekatos, &m_spearHekatos, &m_greatbladeHekatos};
    }

    bool MortekGuard::configure(int numModels, WeaponOption option, int numGreatblades, bool necrophoros) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }
        const int maxGreatblades = numModels / 10;
        if (numGreatblades > maxGreatblades) {
            // Invalid weapon configuration.
            return false;
        }

        auto hekatos = new Model(BASESIZE, wounds());
        if (numGreatblades) {
            hekatos->addMeleeWeapon(&m_greatbladeHekatos);
            numGreatblades--;
        } else if (option == NadiriteBladeAndShield) {
            hekatos->addMeleeWeapon(&m_bladeHekatos);
        } else if (option == NadirateSpearAndShield) {
            hekatos->addMeleeWeapon(&m_spearHekatos);
        }
        addModel(hekatos);
        for (auto i = 0; i < numGreatblades; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_greatblade);
            addModel(model);
        }

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (option == NadiriteBladeAndShield) {
                model->addMeleeWeapon(&m_bladeHekatos);
            } else if (option == NadirateSpearAndShield) {
                model->addMeleeWeapon(&m_spearHekatos);
            }
            addModel(model);
        }

        m_necrophoros = necrophoros;

        m_points = ComputePoints(numModels);

        return true;
    }

    int MortekGuard::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        if ((unmodifiedHitRoll == 6) &&
            ((weapon->name() == m_blade.name()) || (weapon->name() == m_spear.name())))
            return 2;
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int MortekGuard::runModifier() const {
        auto mod = Unit::runModifier();
        if (m_necrophoros) mod++;
        return mod;
    }

    int MortekGuard::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (m_necrophoros) mod++;
        return mod;
    }

    int MortekGuard::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace OssiarchBonereapers
