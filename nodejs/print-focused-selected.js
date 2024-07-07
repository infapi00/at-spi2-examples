/*
 * Usage:
 *    node print-focused-selected.js <filtername>
 *
 * For each focus/selection change, it prints the following for each
 * object getting the focus/selection:
 *    (appname, name, rolename, [state-set])
 *
 * <filtername> is an optional parameter. If provided will only print
 * the events for a given application. If not it will print events for
 * any application.
 *
 *
 * NOTES: print-focused-selected.c uses stateSet.get_states. But on
 * javascript, calling that method fails. A bug is needed to track
 * that problem. FWIW, that also affects pyatspi2, as the array is
 * redone manually there.
 */

import gi from "node-gtk";

const Atspi = gi.require( 'Atspi' );

let filterName;

/* FIXME: atspi lacks this method (available on atk). Instead of
 * implementing it, lets wait until atspi has that method.
 */
function atspiStateGetName(state) {
    return state;
}

/*
 * Returns @accessible state set as a string
 */
function getStateSetString(accessible) {
    let stateSet = accessible.getStateSet();
    let result = "[";
    let i = 0;

    for (i = 0; i < Atspi.StateType.LAST_DEFINED; i++)
        if (stateSet.contains(i))
            result = result + atspiStateGetName(i) + " ";

    result += "]";

  return result;
}

function getLabel(accessible) {
    let relationSet;
    let i = 0;

    relationSet = null; // Bug when calling accessible.getRelationSet();
    if (!relationSet)
        return "NULL";

    /* something like "let relation in relationSet" doesn't work, and
     * it seems that GArray "len" is not exposed */
    while (relationSet[i]) {
        let relation = relationSet[i];

        if (relation.getRelationType() == Atspi.RelationType.LABELLED_BY)
            return relation.getTarget(0).getName();

        i++;
    }

    return "NULL";
}

function printInfo(accessible, appName) {
    let name;
    let roleName = "NULL";
    let stateSetString = "NULL";

    name = accessible.getName();
    if (!name)
        name = getLabel(accessible);
    roleName = accessible.getRoleName();

    stateSetString = getStateSetString(accessible);

    console.log("("+appName+", "+name+", "+roleName+", "+stateSetString+")");
}

function onChanged(event) {
    let application;
    let appName;

    if (!event.source)
        return;

    /* we only care about focus/selection gain */
    if (!event.detail1)
        return;

    application = event.source.getApplication();
    if (!application)
        return;

    appName = application.getName();
    if (filterName != null && filterName != appName)
        return;

    printInfo(event.source, appName);
}


if (process.argv[2]) {
    filterName = process.argv[2];
    print(filterName);
} else {
    console.log("NOTE: Application name to filter not specified. Showing "+
          "focus/selection changes for any application.\n");
}

Atspi.init();
let atspiListener = Atspi.EventListener.new(onChanged);
atspiListener.register("object:state-changed:focused");
atspiListener.register("object:state-changed:selected");
Atspi.eventMain();
