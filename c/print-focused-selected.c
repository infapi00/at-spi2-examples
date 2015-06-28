/*
 * For each focus/selection change, it prints the (name, role, [state-set])
 * for the object getting the focus/selection.
 *
 * You can specify if you want to filter by application. Use --help
 * for more information.
 */

#include <atspi/atspi.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

gchar *filter_name = NULL;

const static gchar*
atspi_state_get_name (gint state)
{
  GTypeClass *type_class;
  GEnumValue *value;

  type_class = g_type_class_ref (ATSPI_TYPE_STATE_TYPE);
  g_return_val_if_fail (G_IS_ENUM_CLASS (type_class), "");

  value = g_enum_get_value (G_ENUM_CLASS (type_class), state);

  return value->value_nick;
}

static gchar*
get_state_set (AtspiAccessible *accessible)
{
  AtspiStateSet *state_set = atspi_accessible_get_state_set (accessible);
  GArray *states = atspi_state_set_get_states (state_set);
  gchar *result = g_strdup_printf ("[");
  gchar *aux = NULL;
  gint i;
  AtspiStateType state;

  for (i = 0; i < states->len; i++) {
    state = g_array_index (states, gint, i);

    aux = result;
    if (i < states->len -1)
      result = g_strdup_printf ("%s%s,", aux, atspi_state_get_name(state));
    else
      result = g_strdup_printf ("%s%s", aux, atspi_state_get_name(state));
    g_free (aux);
  }

  aux = result;
  result = g_strconcat (aux, "]", NULL);
  g_free (aux);

  g_array_free (states, TRUE);
  g_object_unref (state_set);

  return result;
}

static gchar*
get_label (AtspiAccessible *accessible)
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

static void
print_info (AtspiAccessible *accessible,
            gchar *app_name)
{
  gchar *name = "NULL";
  gchar *role_name = "NULL";
  gchar *state_set = NULL;

  if (accessible != NULL) {
    name = atspi_accessible_get_name (accessible, NULL);
    if ((name == NULL) || (g_strcmp0 (name, "") == 0))
        name = get_label (accessible);

    role_name = atspi_accessible_get_role_name (accessible, NULL);
  }

  state_set = get_state_set (accessible);
  g_print ("(%s, %s, %s, %s)\n", app_name, name, role_name, state_set);
  g_free (state_set);
}

static void
on_event (const AtspiEvent *event,
          void *data)
{
  AtspiAccessible *application = NULL;
  gchar *app_name = NULL;

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

  print_info (event->source, app_name);

clean:
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

  filter_name = parse_args (&argc, &argv);
  if (!filter_name) {
    g_print ("NOTE: Application name to filter not specified. Showing "
             "focus/selection changes for any application.\n");
  }

  atspi_init ();

  listener = atspi_event_listener_new (on_event, NULL, NULL);

  atspi_event_listener_register (listener, "object:state-changed:focused", NULL);
  atspi_event_listener_register (listener, "object:state-changed:selected", NULL);

  atspi_event_main ();

  return 0;
}
