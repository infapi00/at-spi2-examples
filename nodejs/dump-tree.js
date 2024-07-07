/*
 * Usage:
 *    node dump-tree <application-name>
 *
 * Dump the accessibility hiearchy tree for a given application.
 */

import gi from "node-gtk";

const Atspi = gi.require( 'Atspi' );

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

    return "("+name+", "+roleName+")";
}

function dumpNodeContent(node, padding) {
    let newPadding = padding + "  ";

    let nodeInfo = printInfo(node);
    console.log(padding + nodeInfo);

    for (let i = 0; i < node.getChildCount(); i++)
        dumpNodeContent(node.getChildAtIndex(i), newPadding);
}

function dumpApplication(appName) {
    let desktop;
    let found = false;

    Atspi.init();

    desktop = Atspi.getDesktop (0);
    for(let i =0; i < desktop.getChildCount(); i++) {
        let app = desktop.getChildAtIndex(i);
        if (app.getName() == appName) {
            found = true;
            dumpNodeContent(app, "  ");
        }
    }

    if (!found) {
        console.error("ERROR: Application "+appName+" not found");
    }
}

if (process.argv[2]) {
    let appName = process.argv[2];

    dumpApplication(appName);
} else {
    console.log("ERROR: We only dump the content of a specific app, specify the name");
}

