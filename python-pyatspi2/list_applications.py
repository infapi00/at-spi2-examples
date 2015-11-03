#
# List the applications registered on at-spi2.
#

import pyatspi

desktop = pyatspi.Registry.getDesktop(0);

for app in desktop:
    print(app.name,app.childCount);
