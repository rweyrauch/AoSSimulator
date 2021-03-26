/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/MournfangPack.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 6;
    static const int g_minUnitSize = 2;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = 840;

    bool MournfangPack::s_registered = false;

    bool MournfangPack::AreValid(const ParameterList &parameters) {
        const int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return ((numModels >= g_minUnitSize) && (numModels <= g_maxUnitSize));
    }

    Unit *MournfangPack::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
            int numModels = GetIntParam("Models", parameters, g_minUnitSize);
            auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Culling_Club_Or_Prey_Hacker_And_Ironfist);
            bool banner = GetBoolParam("Banner Bearer", parameters, true);
            bool hornblower = GetBoolParam("Hornblower", parameters, true);
            return new MournfangPack(tribe, numModels, weapons, hornblower, banner);
        }
        return nullptr;
    }

    void MournfangPack::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Culling_Club_Or_Prey_Hacker_And_Ironfist, Gargant_Hacker};
            static FactoryMethod factoryMethod = {
                    MournfangPack::Create,
                    MournfangPack::ValueToString,
                    MournfangPack::EnumStringToInt,
                    MournfangPack::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Culling_Club_Or_Prey_Hacker_And_Ironfist, weapons),
                            BoolParameter("Banner Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe)
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Mournfang Pack", factoryMethod);
        }
    }

    MournfangPack::MournfangPack(Mawtribe tribe, int numModels, WeaponOption weaponOption, bool hornblower, bool bannerBearer) :
            MawtribesBase(tribe, "Mournfang Pack", 9, g_wounds, 6, 4, false) {
        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, MOURNFANG_PACK};
        m_weapons = {&m_pistol, &m_clubOrHacker, &m_gargantHacker, &m_tusks};
        m_hasMount = true;
        m_tusks.setMount(true);

        m_option = weaponOption;

        auto skalg = new Model(g_basesize, wounds());
        skalg->addMissileWeapon(&m_pistol);
        if (weaponOption == Culling_Club_Or_Prey_Hacker_And_Ironfist)
            skalg->addMeleeWeapon(&m_clubOrHacker);
        else if (weaponOption == Gargant_Hacker)
            skalg->addMeleeWeapon(&m_gargantHacker);
        skalg->addMeleeWeapon(&m_tusks);
        addModel(skalg);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weaponOption == Culling_Club_Or_Prey_Hacker_And_Ironfist)
                model->addMeleeWeapon(&m_clubOrHacker);
            else if (weaponOption == Gargant_Hacker)
                model->addMeleeWeapon(&m_gargantHacker);
            model->addMeleeWeapon(&m_tusks);
            if (hornblower) {
                model->setName(Model::Hornblower);
                hornblower = false;
            }
            else if (bannerBearer) {
                model->setName(Model::BannerBearer);
                bannerBearer = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    int MournfangPack::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    std::string MournfangPack::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Culling_Club_Or_Prey_Hacker_And_Ironfist)
                return "Culling Club or Prey Hacker and Ironfist";
            else if (parameter.intValue == Gargant_Hacker) return "Gargant Hacker";
        }
        return MawtribesBase::ValueToString(parameter);
    }

    int MournfangPack::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Culling Club or Prey Hacker and Ironfist") return Culling_Club_Or_Prey_Hacker_And_Ironfist;
        else if (enumString == "Gargant Hacker") return Gargant_Hacker;
        return MawtribesBase::EnumStringToInt(enumString);
    }

    Wounds MournfangPack::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Mournfang Charge
        if (m_charged && (weapon->name() == m_tusks.name())) {
            return {weapon->damage() + 1, 0};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Wounds MournfangPack::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        // Ironfist
        if ((saveRoll == 6) && (m_option == Culling_Club_Or_Prey_Hacker_And_Ironfist)) {
            return {0, 1};
        }
        return Unit::computeReturnedDamage(weapon, saveRoll);
    }

    int MournfangPack::chargeModifier() const {
        auto modifier = Unit::chargeModifier();
        if (isNamedModelAlive(Model::Hornblower)) modifier += 1;
        return modifier;
    }

    int MournfangPack::braveryModifier() const {
        auto mod = MawtribesBase::braveryModifier();
        if (isNamedModelAlive("Banner Bearer")) mod++;
        return mod;
    }

} // namespace OgorMawtribes
