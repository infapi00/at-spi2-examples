import Atspi from "@girs/atspi-2.0";

function getLabel(accessible: Atspi.Accessible) {
  const relationSet = accessible.get_relation_set();
  if (!relationSet) return null;

  for (let relation of relationSet) {
    if (relation.get_relation_type() === Atspi.RelationType.LABELLED_BY)
      return relation.get_target(0).get_name();
  }

  return null;
}

function printInfo(accessible: Atspi.Accessible) {
  let name = accessible.get_name();
  if (!name) name = getLabel(accessible);
  const roleName = accessible.get_role_name()!;

  return `(${name}, ${roleName})`;
}

function dumpNodeContent(node: Atspi.Accessible, padding: string) {
  let newPadding = padding + "  ";

  const nodeInfo = printInfo(node);

  print(padding + nodeInfo);

  for (let i = 0; i < node.get_child_count(); i++) {
    const child: Atspi.Accessible | null = node.get_child_at_index(i);

    if (!child) continue;

    if (child.get_state_set().contains(Atspi.StateType.VISIBLE))
      dumpNodeContent(child, newPadding);
  }
}

function main() {
  Atspi.init();
  const name = ARGV[0];

  const desktop = Atspi.get_desktop(0);
  for (let i = 0, app; (app = desktop.get_child_at_index(i)); i++)
    if (app.get_name() === name) dumpNodeContent(app, "  ");
}

main();
