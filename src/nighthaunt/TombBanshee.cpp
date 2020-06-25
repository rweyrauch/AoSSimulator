/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "nighthaunt/TombBanshee.h"
#include "NighthauntPrivate.h"

namespace Nighthaunt {
    static const int BASESIZE = 25;
    static const int POINTS_PER_UNIT = 80;
    static const int WOUNDS = 4;

    bool TombBanshee::s_registered = false;

    Unit *TombBanshee::Create(const ParameterList &parameters) {
        auto unit = new TombBanshee();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void TombBanshee::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TombBanshee::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    TombBanshee::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Tomb Banshee", factoryMethod);
        }
    }

    TombBanshee::TombBanshee() :
            Nighthaunt("Tomb Banshee", 6, WOUNDS, 10, 4, true),
            m_dagger(Weapon::Type::Melee, "Chill Dagger", 1, 1, 4, 3, -2, RAND_D3) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, TOMB_BANSHEE};
        m_weapons = {&m_dagger};
    }

    bool TombBanshee::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_dagger);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    Wounds TombBanshee::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll == 6) && (weapon->name() == m_dagger.name())) {
            return {0, Dice::rollD3()};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void TombBanshee::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        // Ghostly Howl
        if (player == owningPlayer()) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 10.0);
            if (!units.empty()) {
                const auto roll = Dice::roll2D6();
                if (roll > units[0]->bravery()) {
                    units[0]->applyDamage({0, units[0]->bravery() - roll});
                }
            }
        }
    }

    int TombBanshee::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Nighthaunt
