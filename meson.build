project('bazylum', 'c')
common_src = ['utils.c', 'config.c', 'db.c', 'list.c']
sqlite_dep = dependency('sqlite3')
x11_dep = dependency('x11')
executable('bazylumd', ['bazylumd.c'] + common_src, dependencies : [x11_dep, sqlite_dep])
executable('bazylum', ['bazylum.c'] + common_src, dependencies : [sqlite_dep])
