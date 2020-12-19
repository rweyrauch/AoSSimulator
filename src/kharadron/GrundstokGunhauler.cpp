/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/GrundstokGunhauler.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KharadronPrivate.h"

namespace KharadronOverlords {
    static const int g_basesize = 0;
    static const int g_wounds = 0;
    static const int g_pointsPerUnit = 130;

    bool GrundstokGunhauler::s_registered = false;

    Unit *GrundstokGunhauler::Create(const ParameterList &parameters) {
        auto unit = new GrundstokGunhauler();
        auto option = (WeaponOption) GetEnumParam("Weapon", parameters, Sky_Cannon);

        auto port = (Skyport) GetEnumParam("Skyport", parameters, g_skyport[0]);
        unit->setSkyport(port);

        auto artycle = (Artycle) GetEnumParam("Artycle", parameters, g_artycles[0]);
        auto amendment = (Amendment) GetEnumParam("Amendment", parameters, g_amendments[0]);
        auto footnote = (Footnote) GetEnumParam("Footnote", parameters, g_footnotes[0]);
        unit->setCode(artycle, amendment, footnote);

        auto endrinwork = (Endrinwork) GetEnumParam("Endrinwork", parameters, g_gunhaulerEndrinworks[0]);

        bool ok = unit->configure(option, endrinwork);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string GrundstokGunhauler::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Sky_Cannon) return "Sky Cannon";
            else if (parameter.intValue == Drill_Cannon) return "Drill Cannon";
        }
        return KharadronBase::ValueToString(parameter);
    }

    int GrundstokGunhauler::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Sky Cannon") return Sky_Cannon;
        else if (enumString == "Drill Cannon") return Drill_Cannon;
        return KharadronBase::EnumStringToInt(enumString);
    }

    void GrundstokGunhauler::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Sky_Cannon, Drill_Cannon};
            static FactoryMethod factoryMethod = {
                    GrundstokGunhauler::Create,
                    GrundstokGunhauler::ValueToString,
                    GrundstokGunhauler::EnumStringToInt,
                    GrundstokGunhauler::ComputePoints,
                    {
                            EnumParameter("Weapon", Sky_Cannon, weapons),
                            EnumParameter("Skyport", g_skyport[0], g_skyport),
                            EnumParameter("Artycle", g_artycles[0], g_artycles),
                            EnumParameter("Amendment", g_amendments[0], g_amendments),
                            EnumParameter("Footnote", g_footnotes[0], g_footnotes),
                            EnumParameter("Endrinworks", g_gunhaulerEndrinworks[0], g_gunhaulerEndrinworks)
                    },
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };
            s_registered = UnitFactory::Register("Grundstok Gunhauler", factoryMethod);
        }
    }

    GrundstokGunhauler::GrundstokGunhauler() :
            KharadronBase("Grundstok Gunhauler", 12, g_wounds, 7, 4, true),
            m_cannonShrapnel(),
            m_cannonShell(),
            m_drillCannon(),
            m_carbines(),
            m_boardingWeapons() {
        m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, WAR_MACHINE, SKYVESSEL, GRUNDSTOK_GUNHAULER};
        m_weapons = {&m_cannonShrapnel,
                     &m_cannonShell,
                     &m_drillCannon,
                     &m_carbines,
                     &m_boardingWeapons};
    }

    bool GrundstokGunhauler::configure(WeaponOption option, Endrinwork endrinwork) {
        auto model = new Model(g_basesize, wounds());
        if (option == Sky_Cannon) {
            model->addMissileWeapon(&m_cannonShrapnel);
            model->addMissileWeapon(&m_cannonShell);
            m_cannonShell.activate(false);
        } else if (option == Drill_Cannon) {
            model->addMissileWeapon(&m_drillCannon);
        }
        model->addMissileWeapon(&m_carbines);
        model->addMeleeWeapon(&m_boardingWeapons);
        addModel(model);

        m_endrinwork = endrinwork;

        m_points = g_pointsPerUnit;

        return true;
    }

    int GrundstokGunhauler::moveModifier() const {
        auto mod = Unit::moveModifier();

        if (m_aheadFull) mod += 6;
        m_aheadFull = false;

        return mod;
    }

    void GrundstokGunhauler::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        // Bomb Racks
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && (distanceTo(unit) <= 1.0)) {
            if (Dice::RollD6() >= 4) {
                unit->applyDamage({0, Dice::RollD3()}, this);
            }
        }
    }

    Wounds
    GrundstokGunhauler::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Drill Cannon
        if ((hitRoll >= 5) && (weapon->name() == m_drillCannon.name())) {
            return {0, 3};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void GrundstokGunhauler::onStartMovement(PlayerId player) {
        Unit::onStartMovement(player);

        if (!m_usedAheadFull) {
            // Go ahead and use the extra movement immediately.
            m_aheadFull = true;
            m_usedAheadFull = true;
        }
    }

    int GrundstokGunhauler::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //KharadronOverlords
