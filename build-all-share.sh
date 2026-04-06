meson setup builddir-feature-shared --wipe -Dthorvg_subproject=thorvg-feature -Dthreads=true --optimization=s --buildtype=release -Dthorvg_linkage=shared
meson compile -C builddir-feature-shared

meson setup builddir-main-shared --wipe -Dthorvg_subproject=thorvg-main -Dthreads=true --optimization=s --buildtype=release -Dthorvg_linkage=shared
meson compile -C builddir-main-shared
