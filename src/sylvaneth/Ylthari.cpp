/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <sylvaneth/SylvanethSpells.h>
#include "sylvaneth/Ylthari.h"
#include "SylvanethPrivate.h"

namespace Sylvaneth {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 180;

    bool Ylthari::s_registered = false;

    Unit *Ylthari::Create(const ParameterList &parameters) {
        auto unit = new Ylthari();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfTheDeepwood[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Ylthari::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Ylthari::Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    Ylthari::ComputePoints,
                    {
                            EnumParameter("Lore", g_loreOfTheDeepwood[0], g_loreOfTheDeepwood),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Ylthari", factoryMethod);
        }
    }

    Ylthari::Ylthari() :
            SylvanethBase("Ylthari", 5, g_wounds, 7, 5, false),
            m_briarStaff(Weapon::Type::Melee, "Briar Staff", 1, 1, 3, 3, -1, RAND_D3),
            m_thorns(Weapon::Type::Melee, "Spiteful Thorns", 1, 3, 4, 4, 0, 1),
            m_snappingMandibles(Weapon::Type::Melee, "Snapping Mandibles", 1, 1, 4, 4, -1, 1) {
        m_keywords = {ORDER, SYLVANETH, OAKENBROW, HERO, WIZARD, THORNWYCH, YLTHARI};
        m_weapons = {&m_briarStaff, &m_thorns, &m_snappingMandibles};
        m_battleFieldRole = Leader;

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool Ylthari::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_briarStaff);
        model->addMeleeWeapon(&m_thorns);
        model->addMeleeWeapon(&m_snappingMandibles);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateTheReaping(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    int Ylthari::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Sylvaneth