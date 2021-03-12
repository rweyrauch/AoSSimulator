/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/ArkanautCompany.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KharadronPrivate.h"

namespace KharadronOverlords {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 90;
    static const int g_pointsMaxUnitSize = 360;

    bool ArkanautCompany::s_registered = false;

    Unit *ArkanautCompany::Create(const ParameterList &parameters) {
        auto unit = new ArkanautCompany();
        int numModel = GetIntParam("Models", parameters, g_minUnitSize);
        int numVolleyGuns = GetIntParam("Volley Guns", parameters, 1);
        int numSkyhooks = GetIntParam("Light Skyhooks", parameters, 1);
        int numSkypikes = GetIntParam("Skypikes", parameters, 1);
        auto option = (CaptainWeapon) GetEnumParam("Captain Weapon", parameters, Aetherflare_Pistol);

        auto port = (Skyport) GetEnumParam("Skyport", parameters, g_skyport[0]);
        unit->setSkyport(port);

        auto artycle = (Artycle) GetEnumParam("Artycle", parameters, g_artycles[0]);
        auto amendment = (Amendment) GetEnumParam("Amendment", parameters, g_amendments[0]);
        auto footnote = (Footnote) GetEnumParam("Footnote", parameters, g_footnotes[0]);
        unit->setCode(artycle, amendment, footnote);

        bool ok = unit->configure(numModel, numVolleyGuns, numSkyhooks, numSkypikes, option);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string ArkanautCompany::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Captain Weapon") {
            if (parameter.intValue == Aetherflare_Pistol) return "Aetherflare Pistol";
            else if (parameter.intValue == Privateer_Pistol) return "Privateer Pistol";
        }
        return KharadronBase::ValueToString(parameter);
    }

    int ArkanautCompany::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Privateer Pistol") return Privateer_Pistol;
        else if (enumString == "Aetherflare Pistol") return Aetherflare_Pistol;
        return KharadronBase::EnumStringToInt(enumString);
    }

    void ArkanautCompany::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Aetherflare_Pistol, Volley_Pistol, Privateer_Pistol};
            static FactoryMethod factoryMethod = {
                    ArkanautCompany::Create,
                    ArkanautCompany::ValueToString,
                    ArkanautCompany::EnumStringToInt,
                    ArkanautCompany::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            IntegerParameter("Volley Guns", 1, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Light Skyhooks", 1, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Skypikes", 1, 0, g_maxUnitSize / g_minUnitSize, 1),
                            EnumParameter("Captain Weapon", Aetherflare_Pistol, weapons),
                            EnumParameter("Skyport", g_skyport[0], g_skyport),
                            EnumParameter("Artycle", g_artycles[0], g_artycles),
                            EnumParameter("Amendment", g_amendments[0], g_amendments),
                            EnumParameter("Footnote", g_footnotes[0], g_footnotes),
                    },
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };
            s_registered = UnitFactory::Register("Arkanaut Company", factoryMethod);
        }
    }

    ArkanautCompany::ArkanautCompany() :
            KharadronBase("Arkanaut Company", 4, g_wounds, 6, 4, false),
            m_privateerPistol(Weapon::Type::Missile, "Privateer Pistol", 9, 2, 4, 4, 0, 1),
            m_volleyGun(Weapon::Type::Missile, "Aethermatic Volley Gun", 12, 6, 5, 4, -1, 1),
            m_skyhook(Weapon::Type::Missile, "Light Skyhook", 18, 1, 4, 3, -2, RAND_D3),
            m_aetherflarePistol(Weapon::Type::Missile, "Aetherflare Pistol", 9, 2, 4, 3, 0, 1),
            m_volleyPistol(Weapon::Type::Missile, "Volley Pistol", 9, 3, 4, 4, 0, 1),
            m_cutter(Weapon::Type::Melee, "Arkanaut Cutter", 1, 1, 4, 4, 0, 1),
            m_gunButt(Weapon::Type::Melee, "Gun Butt", 1, 1, 4, 5, 0, 1),
            m_skypike(Weapon::Type::Melee, "Skypike", 2, 2, 4, 4, -1, RAND_D3) {
        m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, SKYFARER, MARINE, ARKANAUT_COMPANY};
        m_weapons = {&m_privateerPistol, &m_volleyGun, &m_skyhook, &m_aetherflarePistol, &m_volleyPistol, &m_cutter,
                     &m_gunButt, &m_skypike};
        m_battleFieldRole = Role::Battleline;
    }

    bool ArkanautCompany::configure(int numModels, int numVolleyGuns, int numSkyhooks, int numSkypikes,
                                    CaptainWeapon option) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }
        const int maxSpecials = numModels / g_minUnitSize;
        if (numVolleyGuns > maxSpecials || numSkyhooks > maxSpecials || numSkypikes > maxSpecials) {
            return false;
        }

        // Add the Captain
        auto captain = new Model(g_basesize, wounds());
        if (option == Aetherflare_Pistol) {
            captain->addMissileWeapon(&m_aetherflarePistol);
        } else if (option == Volley_Pistol) {
            captain->addMissileWeapon(&m_volleyPistol);
        } else if (option == Privateer_Pistol) {
            captain->addMissileWeapon(&m_privateerPistol);
        }
        captain->addMeleeWeapon(&m_cutter);
        addModel(captain);

        for (auto i = 0; i < numVolleyGuns; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_volleyGun);
            model->addMeleeWeapon(&m_gunButt);
            addModel(model);
        }
        for (auto i = 0; i < numSkyhooks; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_skyhook);
            model->addMeleeWeapon(&m_gunButt);
            addModel(model);
        }
        for (auto i = 0; i < numSkypikes; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_skypike);
            addModel(model);
        }

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_privateerPistol);
            model->addMeleeWeapon(&m_cutter);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int ArkanautCompany::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int ArkanautCompany::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        // Glory-seekers
        auto obj = Board::Instance()->getNearestObjective(this);
        if (obj && (distanceTo(obj->m_pos) <= 9.0)) {
            mod++;
        }
        return mod;
    }

    Rerolls ArkanautCompany::battleshockRerolls() const {
        // Glory-seekers
        auto obj = Board::Instance()->getNearestObjective(this);
        if (obj && (distanceTo(obj->m_pos) <= 9.0)) {
            return Rerolls::Failed;
        }

        return Unit::battleshockRerolls();
    }

} //KharadronOverlords
