/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/Katakros.h"
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 20;
    static const int g_pointsPerUnit = 500;

    bool Katakros::s_registered = false;

    Unit *Katakros::Create(const ParameterList &parameters) {
        auto unit = new Katakros();

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        unit->setLegion(legion);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string Katakros::ValueToString(const Parameter &parameter) {
        return OssiarchBonereaperBase::ValueToString(parameter);
    }

    int Katakros::EnumStringToInt(const std::string &enumString) {
        return OssiarchBonereaperBase::EnumStringToInt(enumString);
    }

    void Katakros::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Katakros::Create,
                    Katakros::ValueToString,
                    Katakros::EnumStringToInt,
                    Katakros::ComputePoints,
                    {
                            EnumParameter("Legion", g_legion[0], g_legion),
                            BoolParameter("General")
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Orpheon Katakros", factoryMethod);
        }
    }

    Katakros::Katakros() :
            OssiarchBonereaperBase("Orpheon Katakros", 4, g_wounds, 10, 3, false),
            m_indaKhaat(Weapon::Type::Melee, "Inda-Khaat", 1, 1, 3, 3, -3, 3),
            m_shieldImmortis(Weapon::Type::Melee, "The Shield Immortis", 1, 4, 3, 3, -2, 2),
            m_nadiriteDagger(Weapon::Type::Melee, "Nadirite Dagger", 1, 1, 3, 3, -1, 1),
            m_blades(Weapon::Type::Melee, "Nadirite Duelling Blades", 1, 6, 3, 3, -1, 1),
            m_greatblade(Weapon::Type::Melee, "Soulreaver Greatblade", 1, 3, 3, 3, -1, 1),
            m_spiritDagger(Weapon::Type::Melee, "Spirit Dagger", 1, 3, 3, 3, -1, 1) {
        m_keywords = {DEATH, DEATHLORDS, OSSIARCH_BONEREAPERS, MORTIS_PRAETORIANS, LIEGE, HERO, KATAKROS};
        m_weapons = {&m_indaKhaat, &m_shieldImmortis, &m_nadiriteDagger, &m_blades, &m_greatblade, &m_spiritDagger};
        m_battleFieldRole = Leader;
    }

    bool Katakros::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_indaKhaat);
        model->addMeleeWeapon(&m_shieldImmortis);
        model->addMeleeWeapon(&m_nadiriteDagger);
        model->addMeleeWeapon(&m_blades);
        model->addMeleeWeapon(&m_greatblade);
        model->addMeleeWeapon(&m_spiritDagger);

        m_shieldImmortis.activate(false);

        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    int Katakros::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        if ((unmodifiedHitRoll == 6) &&
            ((weapon->name() == m_nadiriteDagger.name()) || (weapon->name() == m_blades.name())))
            return 2;
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    void Katakros::onWounded() {
        Unit::onWounded();

        if (woundsTaken() >= 13) {
            m_shieldImmortis.activate(true);
            m_blades.activate(false);
            m_indaKhaat.setAttacks(4);
        } else if (woundsTaken() >= 8) {
            m_spiritDagger.activate(false);
        } else if (woundsTaken() >= 4) {
            m_greatblade.activate(false);
            m_indaKhaat.setAttacks(2);
        } else if (woundsTaken() >= 2) {
            m_nadiriteDagger.activate(false);
        }
    }

    Wounds Katakros::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Deadly Combination
        if ((hitRoll == 6) && (weapon->name() == m_shieldImmortis.name())) {
            return {weapon->damage(), 2};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int Katakros::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void Katakros::onRestore() {
        Unit::onRestore();

        // Restore table-driven attributes
        onWounded();

        // Defaults
        m_shieldImmortis.activate(false);
        m_blades.activate(true);
        m_indaKhaat.setAttacks(1);
        m_spiritDagger.activate(true);
        m_greatblade.activate(true);
        m_nadiriteDagger.activate(true);
    }

} // namespace OssiarchBonereapers
