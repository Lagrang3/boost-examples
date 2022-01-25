project('Boost Example','cpp')

boost=dependency('boost',modules : ['mpi','serialization'])
mpi = dependency('mpi',language: 'cpp')

executable('example','example.cpp',dependencies: [boost,mpi])
executable('reduce','reduce.cpp',dependencies: [boost,mpi])
executable('error','error.cpp',dependencies: [boost,mpi])
executable('all_to_all','all_to_all.cpp',dependencies: [boost,mpi])
