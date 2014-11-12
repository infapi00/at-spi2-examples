/*
 * Equivalent to print-focused-selected example. Additionally it
 * notifies when a object implementing _VALUE changes their value.
 *
 * As a reference, it was used while working on bug 648623, based on
 * pyatspi one uploaded at bug 706391.
 */

#include "atspi/atspi.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

gchar *filter_name = NULL;

static gchar*
get_label(AtspiAccessible *accessible)
{
  GArray *relations;
  AtspiRelation *relation;
  gint i;

  relations = atspi_accessible_get_relation_set (accessible, NULL);
  if (relations == NULL)
    return "";

  for (i = 0; i < relations->len; i++) {
    relation = g_array_index (relations, AtspiRelation*, i);

    if (atspi_relation_get_relation_type (relation) == ATSPI_RELATION_LABELLED_BY)
      return atspi_accessible_get_name (atspi_relation_get_target (relation, 0), NULL);
  }

  return "";
}

/*
 * In some cases (or at least in the case of gnome-shell), the slider
 * is a child of the object that receives the focus/selection, so we
 * need to search for it.
 */
static AtspiAccessible*
find_child_slider(AtspiAccessible *accessible)
{
  AtspiAccessible *child = NULL;
  int i = 0;

  for (i = 0; i < atspi_accessible_get_child_count (accessible, NULL); i++) {
    child = atspi_accessible_get_child_at_index (accessible, i, NULL);
    if (atspi_accessible_get_role (child, NULL) == ATSPI_ROLE_SLIDER)
      break;
  }

  return child;
}

static void
print_accessible_info (AtspiAccessible *accessible,
                       gchar *app_name)
{
  gchar *name = "NULL";
  gchar *role_name = "NULL";
  gchar *value_info = NULL;
  AtspiValue *value_iface = NULL;

  if (accessible != NULL) {
    name = atspi_accessible_get_name (accessible, NULL);
    if ((name == NULL) || (g_strcmp0 (name, "") == 0))
        name = get_label (accessible);

    role_name = atspi_accessible_get_role_name (accessible, NULL);
    value_iface = atspi_accessible_get_value_iface (accessible);
    if (value_iface != NULL) {
      value_info = g_strdup_printf ("%f,%f,%f,%f",
                                    atspi_value_get_current_value (value_iface, NULL),
                                    atspi_value_get_minimum_value (value_iface, NULL),
                                    atspi_value_get_maximum_value (value_iface, NULL),
                                    atspi_value_get_minimum_increment (value_iface, NULL));
      if (atspi_value_get_current_value (value_iface, NULL) < 0.5)
        atspi_value_set_current_value (value_iface, 0.5, NULL);
    }
    else
      value_info = g_strdup_printf ("NO VALUE");
  }

  g_print ("(%s, %s, %s, (%s))", app_name, name, role_name, value_info);
  g_free (value_info);
}

static void
on_notify (const AtspiEvent *event, void *data)
{
  AtspiAccessible *application = NULL;
  gchar *app_name = NULL;

  application = atspi_accessible_get_application (event->source, NULL);
  if (application == NULL)
    return;

  app_name = atspi_accessible_get_name (application, NULL);

  if ((filter_name != NULL) && (g_strcmp0 (app_name, filter_name) != 0))
    goto clean;

  g_print ("Change on accessible value: ");
  print_accessible_info (event->source, app_name);
  g_print ("\n");

 clean:
  if (app_name)
    g_free (app_name);
}

static void
on_event (const AtspiEvent *event, void *data)
{
  AtspiAccessible *application = NULL;
  gchar *app_name = NULL;
  AtspiAccessible *slider_child = NULL;

  if (event->source == NULL)
    return;

  /* We only care about focus/selection gain */
  if (!event->detail1)
    return;

  application = atspi_accessible_get_application (event->source, NULL);
  if (application == NULL)
    return;

  app_name = atspi_accessible_get_name (application, NULL);

  if ((filter_name != NULL) && (g_strcmp0 (app_name, filter_name) != 0))
    goto clean;

  print_accessible_info (event->source, app_name);
  slider_child = find_child_slider (event->source);
  print_accessible_info (slider_child, app_name);
  g_print ("\n");

 clean:
  if (app_name)
    g_free (app_name);
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
  AtspiEventListener *listener;
  AtspiEventListener *listener2;

  filter_name = parse_args (&argc, &argv);
  if (!filter_name) {
    g_print ("NOTE: Application name to filter not specified. Showing "
             "focus/selection changes for any application.\n");
  }

  atspi_init ();

  listener = atspi_event_listener_new (on_event, NULL, NULL);
  listener2 = atspi_event_listener_new (on_notify, NULL, NULL);

  atspi_event_listener_register (listener, "object:state-changed:focused", NULL);
  atspi_event_listener_register (listener, "object:state-changed:selected", NULL);
  atspi_event_listener_register (listener2, "object:property-change:accessible-value", NULL);
  atspi_event_main ();

  return 0;
}
