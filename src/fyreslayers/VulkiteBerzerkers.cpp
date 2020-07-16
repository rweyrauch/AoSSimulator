/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/VulkiteBerzerkers.h>
#include <UnitFactory.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 3;

    bool VulkiteBerzerkers::s_registered = false;

    VulkiteBerzerkers::VulkiteBerzerkers() :
            Fyreslayer("Vulkite Berzerkers", 4, WOUNDS, 7, 5, false),
            m_handaxe(Weapon::Type::Melee, "Fyresteel Handaxe", 1, 2, 3, 3, 0, 1),
            m_handaxeKarl(Weapon::Type::Melee, "Fyresteel Handaxe", 1, 3, 3, 3, 0, 1),
            m_warpick(Weapon::Type::Melee, "Fyresteel War-pick", 1, 2, 3, 4, -1, 1),
            m_warpickKarl(Weapon::Type::Melee, "Fyresteel War-pick", 1, 3, 3, 4, -1, 1),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, VULKITE_BERZERKERS};
        m_weapons = {&m_handaxe, &m_handaxeKarl, &m_warpick, &m_warpickKarl, &m_throwingAxe};
        m_battleFieldRole = Battleline;
    }

    bool VulkiteBerzerkers::configure(int numModels, WeaponOption weapons, bool hornOfGrimnir) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_weaponOption = weapons;
        m_hornOfGrimnir = hornOfGrimnir;

        auto karl = new Model(BASESIZE, wounds());
        karl->addMissileWeapon(&m_throwingAxe);
        if (weapons == HandaxeAndShield || weapons == PairedHandaxes) {
            karl->addMeleeWeapon(&m_handaxeKarl);
        } else if (weapons == WarpickAndShield) {
            karl->addMeleeWeapon(&m_warpickKarl);
        }
        addModel(karl);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_throwingAxe);
            if (weapons == HandaxeAndShield || weapons == PairedHandaxes) {
                model->addMeleeWeapon(&m_handaxe);
            } else if (weapons == WarpickAndShield) {
                model->addMeleeWeapon(&m_warpick);
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *VulkiteBerzerkers::Create(const ParameterList &parameters) {
        auto unit = new VulkiteBerzerkers();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, HandaxeAndShield);
        auto horn = GetBoolParam("Horn of Grimnir", parameters, false);

        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        unit->setLodge(lodge);

        bool ok = unit->configure(numModels, weapons, horn);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void VulkiteBerzerkers::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {HandaxeAndShield, WarpickAndShield, PairedHandaxes};
            static FactoryMethod factoryMethod = {
                    VulkiteBerzerkers::Create,
                    VulkiteBerzerkers::ValueToString,
                    VulkiteBerzerkers::EnumStringToInt,
                    VulkiteBerzerkers::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Weapons", HandaxeAndShield, weapons),
                            BoolParameter("Horn of Grimnir"),
                            EnumParameter("Lodge", g_lodge[0], g_lodge),
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Vulkite Berzerkers", factoryMethod);
        }
    }

    std::string VulkiteBerzerkers::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == HandaxeAndShield) { return "Fyresteel Handaxe and Bladed Slingshield"; }
            else if (parameter.intValue == WarpickAndShield) { return "Fyresteel War-pick and Bladed Slingshield"; }
            else if (parameter.intValue == PairedHandaxes) { return "Paired Fyresteel Handaxes"; }
        }
        return Fyreslayer::ValueToString(parameter);
    }

    int VulkiteBerzerkers::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Fyresteel Handaxe and Bladed Slingshield") { return HandaxeAndShield; }
        else if (enumString == "Fyresteel War-pick and Bladed Slingshield") { return WarpickAndShield; }
        else if (enumString == "Paired Fyresteel Handaxes") { return PairedHandaxes; }
        return Fyreslayer::EnumStringToInt(enumString);
    }

    Rerolls VulkiteBerzerkers::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if ((weapon->name() == m_handaxe.name()) && (m_weaponOption == PairedHandaxes)) {
            return RerollFailed;
        }
        return Fyreslayer::toHitRerolls(weapon, target);
    }

    int VulkiteBerzerkers::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    int VulkiteBerzerkers::chargeModifier() const {
        auto mod = Fyreslayer::chargeModifier();

        if (m_hornOfGrimnir) mod++;

        return mod;
    }

} //namespace Fyreslayers