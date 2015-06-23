/*
 * Usage:
 *    gjs list-applications.js
 *
 * List the applications registered on at-spi
 *
 */

const Atspi = imports.gi.Atspi;

let desktop;
let app;

Atspi.init();
desktop = Atspi.get_desktop(0);
for (let i = 0; i < desktop.get_child_count(); i++) {
    let app = desktop.get_child_at_index (i);

    print ("(Index, application, application_child_count)=("+
           i + "," + app.get_name() + "," + app.get_child_count()+")");
}
