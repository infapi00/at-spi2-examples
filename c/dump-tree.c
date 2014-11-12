/*
 * Dump the accessibility hierarchy tree for a given application.
 * It shows the (name, role) for each object
 * TODO: show stateset too.
 *
 * To get the name it tries get_name and the labelled_by relationship.
 *
 */

#include <atspi/atspi.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static gchar*
get_label(AtspiAccessible *accessible)
{
  GArray *relations;
  AtspiRelation *relation;
  gint i;
  gchar *result = "";

  relations = atspi_accessible_get_relation_set (accessible, NULL);
  if (relations == NULL) {
    return "";
  }

  for (i = 0; i < relations->len; i++) {
    relation = g_array_index (relations, AtspiRelation*, i);

    if (atspi_relation_get_relation_type (relation) == ATSPI_RELATION_LABELLED_BY) {
      result = atspi_accessible_get_name (atspi_relation_get_target (relation, 0), NULL);
    }
  }

  if (relations != NULL)
    g_array_free (relations, TRUE);

  return result;
}

static gchar*
print_info (AtspiAccessible *accessible)
{
  gchar *name = "NULL";
  gchar *role_name = "NULL";

  if (accessible != NULL) {
    name = atspi_accessible_get_name (accessible, NULL);
    if ((name == NULL) || (g_strcmp0 (name, "") == 0))
        name = get_label (accessible);

    role_name = atspi_accessible_get_role_name (accessible, NULL);
  }

  return g_strdup_printf ("(%s, %s)", name, role_name);
}

static void
dump_node_content (AtspiAccessible *node,
                   gchar *padding)
{
  AtspiAccessible *inner_node = NULL;
  gint c;
  gchar *new_padding = g_strdup_printf ("  %s", padding);
  gchar *string = NULL;

  string = print_info (node);
  g_print ("%s%s\n", padding, string);

  for (c = 0; c < atspi_accessible_get_child_count (node, NULL); c++) {
    inner_node = atspi_accessible_get_child_at_index (node, c, NULL);
    dump_node_content (inner_node, new_padding);
    g_object_unref (inner_node);
  }

  g_free (string);
  g_free (new_padding);
}

static gchar*
parse_args (int    *argc,
            char ***argv)
{
  GError *error = NULL;
  GOptionContext *context;
  static gchar *name = NULL;
  static GOptionEntry entries [] =
    {
      {"application", 'a', 0, G_OPTION_ARG_STRING, &name, "Application name", NULL},
      {NULL,},
    };

  context = g_option_context_new ("");
  g_option_context_add_main_entries (context, entries, NULL);
  if (!g_option_context_parse (context, argc, argv, &error))
    {
      g_print ("%s\n", error->message);
      g_print ("Use --help for more information.\n");
      exit (0);
    }

  return name;
}

int main(int argc, gchar **argv)
{
  gint i;
  AtspiAccessible *desktop = NULL;
  AtspiAccessible *app = NULL;
  gchar *app_name = NULL;
  gboolean found = FALSE;

  app_name = parse_args (&argc, &argv);
  if (!app_name) {
    g_print ("ERROR: We only dump the content of a specific app, specify the name\n");
    return 0;
  }

  atspi_init ();

  desktop = atspi_get_desktop (0);
  for (i = 0; i < atspi_accessible_get_child_count (desktop, NULL); i++) {
    app = atspi_accessible_get_child_at_index (desktop, i, NULL);
    if (!g_strcmp0 (atspi_accessible_get_name (app, NULL), app_name)) {
      found = TRUE;
      dump_node_content (app, "  ");
    }
    g_object_unref (app);
  }

  if (!found) {
    g_print ("ERROR: Application \"%s\" not found\n", app_name);
    return 0;
  }

  return 1;
}
