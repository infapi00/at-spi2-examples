/*
 * Usage:
 *    node list-applications.js
 *
 * List the applications registered on at-spi
 *
 */

import gi from "node-gtk";

const Atspi = gi.require( 'Atspi' );

Atspi.init();

let desktop = Atspi.getDesktop(0);

for (let i = 0; i < desktop.getChildCount(); i++) {
    let app = desktop.getChildAtIndex (i);

    console.log("(Index, application, applicationChildCount)=("+
           i + "," + app.getName() + "," + app.getChildCount()+")");
}
