/*
 * For each focus/selection change, it prints the (name, role)
 * for the object getting the focus/selection.
 */

const Atspi = imports.gi.Atspi;

function onChanged (event) {
    log(event.source.get_name() + ',' + event.source.get_role_name());
}

Atspi.init();
let atspiListener = Atspi.EventListener.new(onChanged);
atspiListener.register("object:state-changed:focused");
atspiListener.register("object:state-changed:selected");
Atspi.event_main();
