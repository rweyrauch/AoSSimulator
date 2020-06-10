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

namespace KharadronOverlords {
    static const int BASESIZE = 0;
    static const int WOUNDS = 0;
    static const int POINTS_PER_UNIT = 150;

    bool GrundstokGunhauler::s_registered = false;

    Unit *GrundstokGunhauler::Create(const ParameterList &parameters) {
        auto unit = new GrundstokGunhauler();
        auto option = (WeaponOption) GetEnumParam("Weapon", parameters, SkyCannon);

        auto port = (Skyport) GetEnumParam("Skyport", parameters, KharadronOverlords::Custom);
        unit->setSkyport(port);

        bool ok = unit->configure(option);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string GrundstokGunhauler::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == SkyCannon) return "Sky Cannon";
            else if (parameter.intValue == DrillCannon) return "Drill Cannon";
        }
        return KharadronBase::ValueToString(parameter);
    }

    int GrundstokGunhauler::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Sky Cannon") return SkyCannon;
        else if (enumString == "Drill Cannon") return DrillCannon;
        return KharadronBase::EnumStringToInt(enumString);
    }

    void GrundstokGunhauler::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {SkyCannon, DrillCannon};
            static FactoryMethod factoryMethod = {
                    GrundstokGunhauler::Create,
                    GrundstokGunhauler::ValueToString,
                    GrundstokGunhauler::EnumStringToInt,
                    GrundstokGunhauler::ComputePoints,
                    {
                            EnumParameter("Weapon", SkyCannon, weapons),
                            EnumParameter("Skyport", g_skyport[0], g_skyport)
                    },
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };
            s_registered = UnitFactory::Register("Grundstok Gunhauler", factoryMethod);
        }
    }

    GrundstokGunhauler::GrundstokGunhauler() :
            KharadronBase("Grundstok Gunhauler", 12, WOUNDS, 7, 4, true),
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

    bool GrundstokGunhauler::configure(WeaponOption option) {
        auto model = new Model(BASESIZE, wounds());
        if (option == SkyCannon) {
            model->addMissileWeapon(&m_cannonShrapnel);
            model->addMissileWeapon(&m_cannonShell);
            m_cannonShell.activate(false);
        } else if (option == DrillCannon) {
            model->addMissileWeapon(&m_drillCannon);
        }
        model->addMissileWeapon(&m_carbines);
        model->addMeleeWeapon(&m_boardingWeapons);
        addModel(model);

        m_points = POINTS_PER_UNIT;

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
            if (Dice::rollD6() >= 4) {
                unit->applyDamage({0, Dice::rollD3()});
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
        return POINTS_PER_UNIT;
    }

} //KharadronOverlords
