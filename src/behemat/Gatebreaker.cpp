/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <behemat/Gatebreaker.h>
#include <UnitFactory.h>
#include "SonsOfBehehmetPrivate.h"

namespace SonsOfBehemat {
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 0;
    static const int POINTS_PER_UNIT = 490;

    bool Gatebreaker::s_registered = false;

    Gatebreaker::Gatebreaker() :
            SonsOfBehematBase("Gatebreaker Mega-Gargant", 8, WOUNDS, 6, 5, false) {
    }

    bool Gatebreaker::configure() {
        return false;
    }

    Unit *Gatebreaker::Create(const ParameterList &parameters) {
        auto unit = new Gatebreaker();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        unit->setCommandTrait(trait);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefact[0]);
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

    void Gatebreaker::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Gatebreaker::Create,
                    SonsOfBehematBase::ValueToString,
                    SonsOfBehematBase::EnumStringToInt,
                    Gatebreaker::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            EnumParameter("Artefact", g_artefact[0], g_artefact),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {SONS_OF_BEHEMAT}
            };
            s_registered = UnitFactory::Register("Gatebreaker Mega-Gargant", factoryMethod);
        }
    }

    int Gatebreaker::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace SonsOfBehemat
