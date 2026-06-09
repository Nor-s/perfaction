meson setup builddir-main --wipe -Dthorvg_subproject=thorvg-main -Dthreads=true --optimization=s
meson compile -C builddir-main
