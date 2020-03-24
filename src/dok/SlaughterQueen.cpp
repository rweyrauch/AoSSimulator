/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/SlaughterQueen.h>
#include <UnitFactory.h>
#include <Board.h>

namespace DaughtersOfKhaine {
    static const int BASESIZE = 25;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    bool SlaughterQueen::s_registered = false;

    SlaughterQueen::SlaughterQueen() :
            DaughterOfKhaine("Slaughter Queen", 6, WOUNDS, 9, 5, false),
            m_bladeOfKhaine(Weapon::Type::Melee, "Blade of Khaine", 1, 4, 3, 4, -1, 1),
            m_deathsword(Weapon::Type::Melee, "Deathsword", 1, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, HERO, PRIEST, SLAUGHTER_QUEEN};
        m_weapons = {&m_bladeOfKhaine, &m_deathsword};

        // Pact of Blood
        m_totalUnbinds = 1;
    }

    bool SlaughterQueen::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_bladeOfKhaine);
        model->addMeleeWeapon(&m_deathsword);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *SlaughterQueen::Create(const ParameterList &parameters) {
        auto unit = new SlaughterQueen();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SlaughterQueen::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    nullptr,
                    nullptr,
                    ComputePoints,
                    {
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Slaughter Queen", factoryMethod);
        }
    }

    int SlaughterQueen::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

    void SlaughterQueen::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        m_bladeOfKhaine.setDamage(1);

        // Priestess of Khaine
        const auto roll = Dice::rollD6();
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && distanceTo(unit) <= 3.0f) {
            // Touch of Death
            if (roll == 1) {
                applyDamage({0, 1});
            }
            else if (roll >= 3) {
                if (Dice::rollD6() >= 4) {
                    unit->applyDamage({0, Dice::rollD3()});
                }
            }
        }
        else {
            // TODO: Dance of Doom

            // Rune of Khorne
            if (roll == 1) {
                applyDamage({0, 1});
            }
            else if (roll >= 3) {
                m_bladeOfKhaine.setDamage(RAND_D3);
            }
        }
    }

} //namespace DaughtersOfKhaine

