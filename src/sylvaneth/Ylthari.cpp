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

namespace Sylvaneth {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 180;

    bool Ylthari::s_registered = false;

    Unit *Ylthari::Create(const ParameterList &parameters) {
        auto unit = new Ylthari();

        bool ok = unit->configure();
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
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Ylthari", factoryMethod);
        }
    }

    Ylthari::Ylthari() :
            SylvanethBase("Ylthari", 5, WOUNDS, 7, 5, false),
            m_briarStaff(Weapon::Type::Melee, "Briar Staff", 1, 1, 3, 3, -1, RAND_D3),
            m_thorns(Weapon::Type::Melee, "Spiteful Thorns", 1, 3, 4, 4, 0, 1),
            m_snappingMandibles(Weapon::Type::Melee, "Snapping Mandibles", 1, 1, 4, 4, -1, 1) {
        m_keywords = {ORDER, SYLVANETH, OAKENBROW, HERO, WIZARD, THORNWYCH, YLTHARI};
        m_weapons = {&m_briarStaff, &m_thorns, &m_snappingMandibles};

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool Ylthari::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_briarStaff);
        model->addMeleeWeapon(&m_thorns);
        model->addMeleeWeapon(&m_snappingMandibles);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateTheReaping(this)));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int Ylthari::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Sylvaneth