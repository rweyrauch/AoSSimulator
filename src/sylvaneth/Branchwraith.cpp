/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <sylvaneth/Branchwraith.h>
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>

namespace Sylvaneth {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 80;

    bool Branchwraith::s_registered = false;

    Branchwraith::Branchwraith() :
            SylvanethBase("Branchwraith", 7, WOUNDS, 8, 5, false),
            m_piercingTalons(Weapon::Type::Melee, "Piercing Talons", 2, 3, 4, 4, -1, 1) {
        m_keywords = {ORDER, SYLVANETH, FOREST_FOLK, HERO, WIZARD, BRANCHWRAITH};
        m_weapons = {&m_piercingTalons};

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool Branchwraith::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_piercingTalons);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *Branchwraith::Create(const ParameterList &parameters) {
        auto unit = new Branchwraith();

        auto glade = (Glade) GetEnumParam("Glade", parameters, SylvanethBase::None);
        unit->setGlade(glade);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Branchwraith::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Branchwraith::Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    Branchwraith::ComputePoints,
                    {
                            {ParamType::Enum, "Glade", SylvanethBase::None, SylvanethBase::None,
                             SylvanethBase::Harvestboon, 1},
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Branchwraith", factoryMethod);
        }
    }

    int Branchwraith::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = Unit::targetHitModifier(weapon, attacker);

        // Blessing of the Forest
        auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), AWAKENED_WYLDWOOD, 6.0f);
        if (unit != nullptr) {
            modifier -= 1;
        }

        return modifier;
    }

    int Branchwraith::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Sylvaneth
