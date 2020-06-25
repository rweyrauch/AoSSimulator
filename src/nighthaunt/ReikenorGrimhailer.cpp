/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "nighthaunt/ReikenorGrimhailer.h"
#include "NighthauntPrivate.h"

namespace Nighthaunt {
    static const int BASESIZE = 75; // x42 oval
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 170;

    bool ReikenorTheGrimhailer::s_registered = false;

    Unit *ReikenorTheGrimhailer::Create(const ParameterList &parameters) {
        auto unit = new ReikenorTheGrimhailer();

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ReikenorTheGrimhailer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ReikenorTheGrimhailer::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    ReikenorTheGrimhailer::ComputePoints,
                    {
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Reikenor the Grimhailer", factoryMethod);
        }
    }

    ReikenorTheGrimhailer::ReikenorTheGrimhailer() :
            Nighthaunt("Reikenor the Grimhailer", 14, WOUNDS, 10, 4, true),
            m_fellreaper(Weapon::Type::Melee, "Fellreaper", 2, 4, 4, 3, -1, 2),
            m_hoovesAndTeeth(Weapon::Type::Melee, "Ghostly Hooves and Teeth", 1, 3, 4, 4, 0, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, WIZARD, REIKENOR_THE_GRIMHAILER};
        m_weapons = {&m_fellreaper, &m_hoovesAndTeeth};
        m_hasMount = true;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool ReikenorTheGrimhailer::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_fellreaper);
        model->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = ComputePoints(1);

        return true;
    }

    Wounds
    ReikenorTheGrimhailer::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll == 6) && (weapon->name() == m_fellreaper.name())) {
            return {0, 2};
        }

        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Rerolls ReikenorTheGrimhailer::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Reaped Like Corn
        if ((target->remainingModels() >= 5) && (weapon->name() == m_fellreaper.name())) {
            return RerollFailed;
        }

        return Unit::toHitRerolls(weapon, target);
    }

    int ReikenorTheGrimhailer::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Nighthaunt
