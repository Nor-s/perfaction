meson setup builddir-feature --wipe -Dthorvg_subproject=thorvg-feature -Dthreads=true --optimization=s --buildtype=release
meson compile -C builddir-feature

meson setup builddir-main --wipe -Dthorvg_subproject=thorvg-main -Dthreads=true --optimization=s --buildtype=release
meson compile -C builddir-main
