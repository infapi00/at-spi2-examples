/*
 * List the applications registered on at-spi.
 */

#include <atspi/atspi.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(int argc, gchar **argv)
{
  gint i;
  AtspiAccessible *desktop = NULL;
  AtspiAccessible *app = NULL;

  atspi_init ();

  desktop = atspi_get_desktop (0);
  for (i = 0; i < atspi_accessible_get_child_count (desktop, NULL); i++) {
    app = atspi_accessible_get_child_at_index (desktop, i, NULL);

    g_print ("(Index, application, application_child_count)=(%d,%s,%d)\n",
             i, atspi_accessible_get_name (app, NULL), atspi_accessible_get_child_count (app, NULL));
    g_object_unref (app);
  }

  return 1;
}
