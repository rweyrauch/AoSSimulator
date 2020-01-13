/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

if (AosSim === undefined) {
    console.log("Module not found");
}
else {
    console.log("AoS module is loaded.");
}

AosSim().then(AosSim => {
    const sim = AosSim.JSInterface.prototype;

    sim.Initialize();

    // Run some basic tests.
    var gac = sim.GrandAllianceStringToKeyword("Chaos");
    var gad = sim.GrandAllianceStringToKeyword("Destruction");

    console.log("GA: " + gac + " " + gad);

    var sce = sim.FactionStringToKeyword("Stormcast Eternal");
    var scetxt = sim.FactionKeywordToString(sce);
    console.log("ScE: " + sce + "  Text: " + scetxt);

    var numUnits = sim.GetNumberOfAvailableUnits();
    console.log("Number of units: " + numUnits);

    for (var i = 0; i < numUnits; i++) {
        var unitInfo = new AosSim.JSUnitInfo();
        sim.GetUnitInfo(i, unitInfo);
        console.log("Name: " + unitInfo.name + " GA: " + unitInfo.grandAlliance + " Factions: " + unitInfo.numberOfFactions + " " +
            sim.FactionKeywordToString(unitInfo.getFaction(0)) + "  Params: " + unitInfo.numberOfParameters);
    }

    var unitInfo = new AosSim.JSUnitInfo();
    sim.GetUnitInfo(0, unitInfo);

    var unit1 = sim.CreateUnit(unitInfo.name, unitInfo.parameters, unitInfo.numberOfParameters);
    if (unit1) {
        console.log("Created a unit!  Name: " + unitInfo.name + "  Points: " + unit1.points());
    }

    sim.GetUnitInfo(1, unitInfo);

    var unit2 = sim.CreateUnit(unitInfo.name, unitInfo.parameters, unitInfo.numberOfParameters);
    if (unit2) {
        console.log("Created a unit!  Name: " + unitInfo.name + "  Points: " + unit2.points());
    }

    var mano = new AosSim.ManoAMano(5);
    if (mano) {

        mano.combatants(unit1, unit2);
        mano.start();
        while (!mano.done()) {
            mano.simulate();

            const round = mano.currentRound();
            mano.next();
        }

        console.log("Create a ManoAMano object.  Current round: " + mano.currentRound() + "  Victor: " + mano.getVictor());
    }

});
