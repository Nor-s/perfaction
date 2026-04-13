meson setup builddir-nors --wipe -Dthorvg_subproject=thorvg-nors -Dthreads=true --optimization=s
meson compile -C builddir-nors

meson setup builddir-nors-false --wipe -Dthorvg_subproject=thorvg-nors -Dthreads=false --optimization=s
meson compile -C builddir-nors-false

meson setup builddir-main --wipe -Dthorvg_subproject=thorvg-main -Dthreads=true --optimization=s
meson compile -C builddir-main
